FROM ubuntu:24.04

ARG PLATFORMIO_VERSION=6.2.0
ARG ESPTOOL_JS_VERSION=0.7.0

ENV DEBIAN_FRONTEND=noninteractive \
	PLATFORMIO_CORE_DIR=/pio \
	HOME=/tmp \
	PYTHONUNBUFFERED=1 \
	ESPTOOL_JS=/opt/esptool-js/bundle.js \
	PATH=/opt/venv/bin:$PATH

RUN apt-get update \
	&& apt-get install -y --no-install-recommends \
		build-essential \
		ca-certificates \
		clang-format \
		clang-tidy \
		curl \
		git \
		libsdl2-dev \
		make \
		python3 \
		python3-venv \
	&& rm -rf /var/lib/apt/lists/*

RUN python3 -m venv /opt/venv \
	&& pip install --no-cache-dir \
		platformio==${PLATFORMIO_VERSION} \
		pillow

RUN mkdir -p /opt/esptool-js \
	&& curl -fsSL https://registry.npmjs.org/esptool-js/-/esptool-js-${ESPTOOL_JS_VERSION}.tgz \
		| tar xz -C /opt/esptool-js --strip-components=1 package/bundle.js package/LICENSE

RUN mkdir -p /pio /work \
	&& chmod 1777 /pio /work

WORKDIR /work

EXPOSE 8080

CMD ["python3", "/work/docker/server.py"]
