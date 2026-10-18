import { ESPLoader, Transport } from "/vendor/esptool-js.js";

const ESPRESSIF_VID = 0x303a;
const FLASH_BAUDRATE = 921600;
const MONITOR_BAUDRATE = 115200;
const MONITOR_REOPEN_DELAY_MS = 500;
const HARD_RESET_SEQUENCE = "D0|R1|W200|R0|W200";
const MAX_CONSOLE_LENGTH = 200000;
const CONSOLE_STORAGE_KEY = "synthwired-console";
const CONSOLE_SAVE_DELAY_MS = 500;

const IMAGE_TITLES = {
	firmware: "Firmware",
	filesystem: "Filesystem",
	factory: "Factory image",
};

const consoleElement = document.getElementById("console");
const portStatus = document.getElementById("port-status");
const progress = document.getElementById("progress");
const monitorButton = document.getElementById("monitor");
const monitorAfterFlash = document.getElementById("monitor-after-flash");
const imagesBody = document.querySelector("#images tbody");

let port = null;
let monitor = null;
let busy = false;
let consoleSaveTimer = null;

const terminal = {
	clean() {
		consoleElement.textContent = "";
		scheduleConsoleSave();
	},
	writeLine(data) {
		writeConsole(`${data}\n`);
	},
	write(data) {
		writeConsole(data);
	},
};

const ACTIONS = {
	"build-firmware": () => runBuild("firmware"),
	"upload-firmware": async () => {
		await runBuild("firmware");
		await flashImages(["firmware"]);
	},
	"upload-firmware-filesystem": async () => {
		await runBuild("firmware");
		await runBuild("filesystem");
		await flashImages(["firmware", "filesystem"]);
	},
	"upload-full": async () => {
		await runBuild("firmware");
		await runBuild("filesystem");
		await flashImages(["factory", "filesystem"]);
	},
};

function writeConsole(text) {
	const isAtBottom = consoleElement.scrollTop + consoleElement.clientHeight >= consoleElement.scrollHeight - 4;
	consoleElement.textContent += text;
	if (consoleElement.textContent.length > MAX_CONSOLE_LENGTH) {
		consoleElement.textContent = consoleElement.textContent.slice(-MAX_CONSOLE_LENGTH / 2);
	}
	if (isAtBottom) {
		consoleElement.scrollTop = consoleElement.scrollHeight;
	}
	scheduleConsoleSave();
}

function saveConsole() {
	clearTimeout(consoleSaveTimer);
	consoleSaveTimer = null;
	sessionStorage.setItem(CONSOLE_STORAGE_KEY, consoleElement.textContent);
}

function scheduleConsoleSave() {
	if (consoleSaveTimer === null) {
		consoleSaveTimer = setTimeout(saveConsole, CONSOLE_SAVE_DELAY_MS);
	}
}

function restoreConsole() {
	consoleElement.textContent = sessionStorage.getItem(CONSOLE_STORAGE_KEY) ?? "";
	consoleElement.scrollTop = consoleElement.scrollHeight;
}

function getHex(value) {
	return `0x${value.toString(16).padStart(6, "0")}`;
}

function getSizeText(size) {
	if (size >= 1024 * 1024) {
		return `${(size / (1024 * 1024)).toFixed(2)} MB`;
	}
	return `${(size / 1024).toFixed(1)} KB`;
}

function getTimeText(seconds) {
	return new Date(seconds * 1000).toLocaleString();
}

function isLinux() {
	const platform = navigator.userAgentData?.platform ?? navigator.platform ?? "";
	return /linux/i.test(platform) && !/android/i.test(navigator.userAgent);
}

function wait(ms) {
	return new Promise((resolve) => setTimeout(resolve, ms));
}

async function getImages() {
	const response = await fetch("/api/images", { cache: "no-store" });
	if (!response.ok) {
		throw new Error(`Image list failed: ${response.status}`);
	}
	return response.json();
}

async function renderImages() {
	const images = await getImages();
	imagesBody.replaceChildren();
	for (const [name, image] of Object.entries(images)) {
		const row = imagesBody.insertRow();
		const cells = [
			IMAGE_TITLES[name] ?? name,
			image.file,
			image.available ? getHex(image.address) : "–",
			image.available ? getSizeText(image.size) : image.error,
			image.available ? getTimeText(image.modified) : "–",
		];
		for (const text of cells) {
			row.insertCell().textContent = text;
		}
	}
}

function updatePortStatus() {
	if (port === null) {
		portStatus.textContent = "No watch selected";
		return;
	}
	const info = port.getInfo();
	portStatus.textContent = `USB ${getHex(info.usbVendorId ?? 0)}:${getHex(info.usbProductId ?? 0)}`;
}

function updateMonitorButton() {
	monitorButton.textContent = monitor === null ? "Start monitor" : "Stop monitor";
}

function setBusy(value) {
	busy = value;
	for (const button of document.querySelectorAll("button")) {
		button.disabled = value;
	}
}

async function selectPort() {
	await stopMonitor();
	port = await navigator.serial.requestPort({ filters: [{ usbVendorId: ESPRESSIF_VID }] });
	updatePortStatus();
}

async function getGrantedPort() {
	const ports = await navigator.serial.getPorts();
	return ports.find((candidate) => candidate.getInfo().usbVendorId === ESPRESSIF_VID) ?? null;
}

async function getPort() {
	if (port === null) {
		port = await getGrantedPort();
		updatePortStatus();
	}
	if (port === null) {
		await selectPort();
	}
	return port;
}

async function getConnectedPort() {
	if (port === null) {
		port = await getGrantedPort();
		updatePortStatus();
	}
	if (port === null) {
		throw new Error("The watch is not connected. Connect it and select it again.");
	}
	return port;
}

async function startMonitor() {
	const selected = await getPort();
	await selected.open({ baudRate: MONITOR_BAUDRATE });
	await selected.setSignals({ dataTerminalReady: false, requestToSend: false });
	const decoder = new TextDecoderStream();
	const session = {
		port: selected,
		pipeClosed: selected.readable.pipeTo(decoder.writable).catch(() => {}),
		reader: decoder.readable.getReader(),
		line: "",
	};
	monitor = session;
	session.closed = readMonitor(session);
	updateMonitorButton();
}

async function stopMonitor() {
	if (monitor === null) {
		return;
	}
	const session = monitor;
	await session.reader.cancel();
	await session.closed;
}

async function readMonitor(session) {
	try {
		for (;;) {
			const { value, done } = await session.reader.read();
			if (done) {
				break;
			}
			writeConsole(value);
			collectBacktraces(session, value);
		}
	} catch (error) {
		terminal.writeLine(`[monitor] ${error.message}`);
	} finally {
		session.reader.releaseLock();
		await session.pipeClosed;
		await session.port.close().catch(() => {});
		if (monitor === session) {
			monitor = null;
		}
		updateMonitorButton();
	}
}

function collectBacktraces(session, text) {
	const lines = (session.line + text).split("\n");
	session.line = lines.pop();
	for (const line of lines) {
		if (line.includes("Backtrace:")) {
			decodeBacktrace(line);
		}
	}
}

async function decodeBacktrace(line) {
	const response = await fetch("/api/decode", { method: "POST", body: line });
	if (!response.ok) {
		return;
	}
	const decoded = await response.text();
	if (decoded.trim() !== "") {
		terminal.write(`\n--- decoded backtrace ---\n${decoded}-------------------------\n`);
	}
}

async function runBuild(target) {
	terminal.writeLine(`\n=== build ${target} ===`);
	const response = await fetch(`/api/build/${target}`, { method: "POST" });
	if (!response.ok) {
		throw new Error(`Build did not start: ${response.status} ${response.statusText}`);
	}
	const reader = response.body.pipeThrough(new TextDecoderStream()).getReader();
	let tail = "";
	for (;;) {
		const { value, done } = await reader.read();
		if (done) {
			break;
		}
		writeConsole(value);
		tail = (tail + value).slice(-64);
	}
	await renderImages();
	if (!/\[exit 0\]\s*$/.test(tail)) {
		throw new Error(`Build of ${target} failed`);
	}
}

async function getFileArray(names) {
	const images = await getImages();
	const fileArray = [];
	for (const name of names) {
		const image = images[name];
		if (!image?.available) {
			throw new Error(`${IMAGE_TITLES[name] ?? name}: ${image?.error ?? "unknown image"}`);
		}
		const response = await fetch(`/api/images/${name}`, { cache: "no-store" });
		if (!response.ok) {
			throw new Error(`Download of ${image.file} failed: ${response.status}`);
		}
		fileArray.push({
			data: new Uint8Array(await response.arrayBuffer()),
			address: image.address,
		});
		terminal.writeLine(`${image.file} → ${getHex(image.address)} (${getSizeText(image.size)})`);
	}
	return fileArray;
}

async function flashImages(names) {
	const fileArray = await getFileArray(names);
	const shouldOpenMonitor = monitor !== null || monitorAfterFlash.checked;
	await stopMonitor();
	const transport = new Transport(await getConnectedPort(), false);
	const loader = new ESPLoader({
		transport,
		baudrate: FLASH_BAUDRATE,
		terminal,
	});
	try {
		await loader.main();
		await loader.writeFlash({
			fileArray,
			flashMode: "keep",
			flashFreq: "keep",
			flashSize: "keep",
			eraseAll: false,
			compress: true,
			reportProgress: (fileIndex, written, total) => {
				progress.value = (fileIndex + written / total) / fileArray.length;
			},
		});
		await loader.after("custom_reset", false, HARD_RESET_SEQUENCE);
	} finally {
		await transport.disconnect().catch(() => {});
	}
	progress.value = 1;
	terminal.writeLine("Flashing done.");
	if (shouldOpenMonitor && port !== null) {
		await wait(MONITOR_REOPEN_DELAY_MS);
		await startMonitor();
	}
}

async function runAction(name) {
	if (busy) {
		return;
	}
	setBusy(true);
	progress.value = 0;
	try {
		if (name.startsWith("upload")) {
			await getPort();
		}
		await ACTIONS[name]();
	} catch (error) {
		terminal.writeLine(`Error: ${error.message}`);
	} finally {
		setBusy(false);
	}
}

async function toggleMonitor() {
	try {
		if (monitor === null) {
			await startMonitor();
		} else {
			await stopMonitor();
		}
	} catch (error) {
		terminal.writeLine(`Error: ${error.message}`);
	}
}

async function init() {
	restoreConsole();
	document.getElementById("linux-snap").hidden = !isLinux();
	window.addEventListener("pagehide", () => {
		if (consoleSaveTimer !== null) {
			saveConsole();
		}
	});

	for (const button of document.querySelectorAll("[data-action]")) {
		button.addEventListener("click", () => runAction(button.dataset.action));
	}
	document.getElementById("clear").addEventListener("click", () => terminal.clean());
	monitorButton.addEventListener("click", toggleMonitor);
	document.getElementById("select-port").addEventListener("click", () => {
		selectPort().catch((error) => terminal.writeLine(`Error: ${error.message}`));
	});

	if ("serial" in navigator) {
		port = await getGrantedPort();
		navigator.serial.addEventListener("connect", (event) => {
			if (port === null && event.target.getInfo().usbVendorId === ESPRESSIF_VID) {
				port = event.target;
				updatePortStatus();
			}
		});
		navigator.serial.addEventListener("disconnect", (event) => {
			if (event.target === port) {
				port = null;
				updatePortStatus();
			}
		});
	} else {
		document.getElementById("unsupported").hidden = false;
	}
	updatePortStatus();

	try {
		await renderImages();
	} catch (error) {
		terminal.writeLine(`Error: ${error.message}`);
	}
}

init();
