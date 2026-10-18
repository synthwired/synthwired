#include "calculator.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "core/ui/colors.h"
#include "core/ui/fonts/digital/20.h"
#include "core/ui/fonts/digital/30.h"

const char *const CalculatorScreen::XML = R"xml(
<component>
	<styles>
		<style
			name="display"
			bg_opa="0"
			bg_color="#color_green_dim"
			border_width="0"
			border_color="#color_green_dark"
			pad_all="0"
			radius="0"
		/>
	</styles>
	<view extends="lv_obj" width="220" height="182" align="top_mid" clickable="false" scrollable="false">
		<style name="container"/>

		<lv_obj
			name="display"
			x="0"
			y="0"
			width="164"
			height="38"
			checked="true"
			clickable="false"
			scrollable="false"
		>
			<style name="display"/>
			<event_cb callback="draw_form_button" trigger="draw_main"/>
			<lv_label
				name="pending"
				text=""
				x="8"
				y="6"
				style_text_font="pixel_8"
				style_text_color="#color_muted"
			/>
			<lv_label
				name="number"
				text="0"
				align="bottom_right"
				x="-7"
				y="-2"
				style_text_font="digital_30"
				style_text_color="#color_green"
			/>
			<lv_label name="sign" text="" style_text_font="pixel_16" style_text_color="#color_green"/>
		</lv_obj>
		<calculator_digit name="backspace" x="168" y="0" height="38">
			<event_cb callback="calculator_draw_backspace" trigger="draw_post"/>
			<event_cb callback="calculator_backspace_clicked" trigger="short_clicked"/>
			<event_cb callback="calculator_backspace_long_pressed" trigger="long_pressed"/>
		</calculator_digit>

		<calculator_digit text="7" x="0" y="44">
			<event_cb callback="calculator_digit_clicked" trigger="clicked"/>
		</calculator_digit>
		<calculator_digit text="8" x="56" y="44">
			<event_cb callback="calculator_digit_clicked" trigger="clicked"/>
		</calculator_digit>
		<calculator_digit text="9" x="112" y="44">
			<event_cb callback="calculator_digit_clicked" trigger="clicked"/>
		</calculator_digit>
		<calculator_operator name="divide" text="÷" x="168" y="44">
			<event_cb callback="calculator_divide_clicked" trigger="clicked"/>
		</calculator_operator>

		<calculator_digit text="4" x="0" y="80">
			<event_cb callback="calculator_digit_clicked" trigger="clicked"/>
		</calculator_digit>
		<calculator_digit text="5" x="56" y="80">
			<event_cb callback="calculator_digit_clicked" trigger="clicked"/>
		</calculator_digit>
		<calculator_digit text="6" x="112" y="80">
			<event_cb callback="calculator_digit_clicked" trigger="clicked"/>
		</calculator_digit>
		<calculator_operator name="multiply" text="×" x="168" y="80">
			<event_cb callback="calculator_multiply_clicked" trigger="clicked"/>
		</calculator_operator>

		<calculator_digit text="1" x="0" y="116">
			<event_cb callback="calculator_digit_clicked" trigger="clicked"/>
		</calculator_digit>
		<calculator_digit text="2" x="56" y="116">
			<event_cb callback="calculator_digit_clicked" trigger="clicked"/>
		</calculator_digit>
		<calculator_digit text="3" x="112" y="116">
			<event_cb callback="calculator_digit_clicked" trigger="clicked"/>
		</calculator_digit>
		<calculator_operator name="subtract" text="-" x="168" y="116">
			<event_cb callback="calculator_subtract_clicked" trigger="clicked"/>
		</calculator_operator>

		<calculator_digit text="0" x="0" y="152">
			<event_cb callback="calculator_digit_clicked" trigger="clicked"/>
		</calculator_digit>
		<calculator_digit text="." font="digital_30" x="56" y="152">
			<event_cb callback="calculator_digit_clicked" trigger="clicked"/>
		</calculator_digit>
		<core_form_button name="equals" text="=" primary="true" x="112" y="152" width="52" height="30">
			<event_cb callback="calculator_equals_clicked" trigger="clicked"/>
		</core_form_button>
		<calculator_operator name="add" text="+" x="168" y="152">
			<event_cb callback="calculator_add_clicked" trigger="clicked"/>
		</calculator_operator>
	</view>
</component>
)xml";

void CalculatorScreen::render()
{
	Screen::render();

	lv_obj_t *const screen = lv_screen_active();
	lv_obj_t *const display = lv_obj_find_by_name(screen, "display");
	if (display == nullptr) {
		return;
	}
	lv_obj_set_user_data(lv_obj_get_parent(display), this);

	this->numberLabel = lv_obj_find_by_name(screen, "number");
	this->signLabel = lv_obj_find_by_name(screen, "sign");
	this->pendingLabel = lv_obj_find_by_name(screen, "pending");
	this->operatorKeys[0] = lv_obj_find_by_name(screen, "add");
	this->operatorKeys[1] = lv_obj_find_by_name(screen, "subtract");
	this->operatorKeys[2] = lv_obj_find_by_name(screen, "multiply");
	this->operatorKeys[3] = lv_obj_find_by_name(screen, "divide");

	this->updateDisplay();
}

const char *CalculatorScreen::getTitle()
{
	return "CALC";
}

bool CalculatorScreen::isFramed()
{
	return true;
}

bool CalculatorScreen::hasBackground()
{
	return false;
}

void CalculatorScreen::registerCallbacks()
{
	lv_xml_register_event_cb(nullptr, "calculator_draw_backspace", CalculatorScreen::drawBackspace);
	lv_xml_register_event_cb(nullptr, "calculator_digit_clicked", CalculatorScreen::digitClicked);
	lv_xml_register_event_cb(nullptr, "calculator_add_clicked", CalculatorScreen::addClicked);
	lv_xml_register_event_cb(nullptr, "calculator_subtract_clicked", CalculatorScreen::subtractClicked);
	lv_xml_register_event_cb(nullptr, "calculator_multiply_clicked", CalculatorScreen::multiplyClicked);
	lv_xml_register_event_cb(nullptr, "calculator_divide_clicked", CalculatorScreen::divideClicked);
	lv_xml_register_event_cb(nullptr, "calculator_equals_clicked", CalculatorScreen::equalsClicked);
	lv_xml_register_event_cb(nullptr, "calculator_backspace_clicked", CalculatorScreen::backspaceClicked);
	lv_xml_register_event_cb(
		nullptr,
		"calculator_backspace_long_pressed",
		CalculatorScreen::backspaceLongPressed
	);
}

void CalculatorScreen::pressDigit(char digit)
{
	if (
		this->error
		|| this->replaceInput
	) {
		this->input[0] = '0';
		this->input[1] = '\0';
		this->error = false;
		this->replaceInput = false;
	}

	const size_t length = std::strlen(this->input);
	const uint8_t digits = CalculatorScreen::getDigitsCount(this->input);

	if (digit == '.') {
		if (std::strchr(this->input, '.') == nullptr) {
			this->input[length] = '.';
			this->input[length + 1] = '\0';
		}
	} else if (std::strcmp(this->input, "0") == 0) {
		this->input[0] = digit;
	} else if (digits < CalculatorScreen::MAX_DIGITS) {
		this->input[length] = digit;
		this->input[length + 1] = '\0';
	}

	this->updateDisplay();
}

uint8_t CalculatorScreen::getDigitsCount(const char *input)
{
	uint8_t digits = 0;
	for (size_t i = 0; input[i] != '\0'; i++) {
		if (input[i] >= '0' && input[i] <= '9') {
			digits++;
		}
	}
	return digits;
}

void CalculatorScreen::pressOperation(Operation next)
{
	if (this->error) {
		return;
	}

	if (this->operation == Operation::NONE) {
		this->saved = std::strtod(this->input, nullptr);
	} else if (!this->replaceInput) {
		this->calculate();
	}

	this->operation = this->error ? Operation::NONE : next;
	this->replaceInput = true;
	this->updateDisplay();
}

void CalculatorScreen::pressEquals()
{
	if (
		this->error
		|| this->operation == Operation::NONE
	) {
		return;
	}

	this->calculate();
	this->operation = Operation::NONE;
	this->replaceInput = true;
	this->updateDisplay();
}

void CalculatorScreen::pressBackspace()
{
	if (
		this->error
		|| (this->replaceInput && this->operation == Operation::NONE)
	) {
		this->clear();
		return;
	}
	if (this->replaceInput) {
		return;
	}

	const size_t length = std::strlen(this->input);
	if (length <= 1) {
		this->input[0] = '0';
		this->input[1] = '\0';
	} else {
		this->input[length - 1] = '\0';
	}

	this->updateDisplay();
}

void CalculatorScreen::clear()
{
	this->input[0] = '0';
	this->input[1] = '\0';
	this->saved = 0.0;
	this->operation = Operation::NONE;
	this->replaceInput = false;
	this->error = false;
	this->updateDisplay();
}

void CalculatorScreen::calculate()
{
	const double current = std::strtod(this->input, nullptr);
	if (
		this->operation == Operation::DIVIDE
		&& current == 0.0
	) {
		this->error = true;
		return;
	}

	double result = this->saved;
	switch (this->operation) {
		case Operation::ADD:
			result += current;
			break;
		case Operation::SUBTRACT:
			result -= current;
			break;
		case Operation::MULTIPLY:
			result *= current;
			break;
		case Operation::DIVIDE:
			result /= current;
			break;
		case Operation::NONE:
			break;
	}

	if (!CalculatorScreen::formatNumber(result, this->input, sizeof(this->input))) {
		this->error = true;
		return;
	}
	this->saved = result;
}

void CalculatorScreen::updateDisplay()
{
	this->updateNumber();
	this->updatePending();

	for (uint8_t i = 0; i < CalculatorScreen::OPERATORS_COUNT; i++) {
		if (this->operatorKeys[i] == nullptr) {
			continue;
		}
		const bool active = !this->error && static_cast<uint8_t>(this->operation) == i + 1;
		lv_obj_set_state(this->operatorKeys[i], LV_STATE_CHECKED, active);
	}
}

void CalculatorScreen::updateNumber()
{
	if (
		this->numberLabel == nullptr
		|| this->signLabel == nullptr
	) {
		return;
	}

	const bool negative = !this->error && this->input[0] == '-';
	const char *const text = this->error ? "ERR" : (negative ? this->input + 1 : this->input);
	const bool large = std::strlen(text) <= CalculatorScreen::LARGE_FONT_MAX_LENGTH;

	lv_label_set_text(this->numberLabel, text);
	lv_obj_set_style_text_font(this->numberLabel, large ? &font_digital_30 : &font_digital_20, LV_PART_MAIN);
	lv_obj_set_style_text_color(
		this->numberLabel,
		lv_color_hex(this->error ? COLOR_MAGENTA : COLOR_GREEN),
		LV_PART_MAIN
	);

	lv_label_set_text(this->signLabel, negative ? "-" : "");
	lv_obj_update_layout(this->numberLabel);
	lv_obj_align_to(
		this->signLabel,
		this->numberLabel,
		LV_ALIGN_OUT_LEFT_MID,
		-CalculatorScreen::SIGN_GAP,
		0
	);
}

void CalculatorScreen::updatePending()
{
	if (this->pendingLabel == nullptr) {
		return;
	}

	char number[CalculatorScreen::INPUT_SIZE];
	if (
		this->error
		|| this->operation == Operation::NONE
		|| !CalculatorScreen::formatNumber(this->saved, number, sizeof(number))
	) {
		lv_label_set_text(this->pendingLabel, "");
		return;
	}

	lv_label_set_text_fmt(
		this->pendingLabel,
		"%s %s",
		number,
		CalculatorScreen::OPERATION_SYMBOLS[static_cast<uint8_t>(this->operation)]
	);
}

bool CalculatorScreen::formatNumber(
	double value,
	char *buffer,
	size_t size
)
{
	const double magnitude = std::fabs(value);
	if (
		!std::isfinite(value)
		|| magnitude >= CalculatorScreen::MAX_VALUE
	) {
		return false;
	}

	const int integerDigits = magnitude < 1.0 ? 1 : static_cast<int>(std::log10(magnitude)) + 1;
	const int decimals = CalculatorScreen::MAX_DIGITS - integerDigits;
	std::snprintf(buffer, size, "%.*f", decimals > 0 ? decimals : 0, value);

	if (std::strchr(buffer, '.') != nullptr) {
		size_t length = std::strlen(buffer);
		while (buffer[length - 1] == '0') {
			length--;
		}
		if (buffer[length - 1] == '.') {
			length--;
		}
		buffer[length] = '\0';
	}

	if (std::strcmp(buffer, "-0") == 0) {
		buffer[0] = '0';
		buffer[1] = '\0';
	}
	return true;
}

void CalculatorScreen::digitClicked(lv_event_t *e)
{
	lv_obj_t *const key = lv_event_get_current_target_obj(e);
	CalculatorScreen *const screen = static_cast<CalculatorScreen *>(
		lv_obj_get_user_data(lv_obj_get_parent(key))
	);
	if (screen == nullptr) {
		return;
	}
	const char *const text = lv_label_get_text(lv_obj_get_child(key, 0));
	screen->pressDigit(text[0]);
}

void CalculatorScreen::addClicked(lv_event_t *e)
{
	lv_obj_t *const key = lv_event_get_current_target_obj(e);
	CalculatorScreen *const screen = static_cast<CalculatorScreen *>(
		lv_obj_get_user_data(lv_obj_get_parent(key))
	);
	if (screen != nullptr) {
		screen->pressOperation(Operation::ADD);
	}
}

void CalculatorScreen::subtractClicked(lv_event_t *e)
{
	lv_obj_t *const key = lv_event_get_current_target_obj(e);
	CalculatorScreen *const screen = static_cast<CalculatorScreen *>(
		lv_obj_get_user_data(lv_obj_get_parent(key))
	);
	if (screen != nullptr) {
		screen->pressOperation(Operation::SUBTRACT);
	}
}

void CalculatorScreen::multiplyClicked(lv_event_t *e)
{
	lv_obj_t *const key = lv_event_get_current_target_obj(e);
	CalculatorScreen *const screen = static_cast<CalculatorScreen *>(
		lv_obj_get_user_data(lv_obj_get_parent(key))
	);
	if (screen != nullptr) {
		screen->pressOperation(Operation::MULTIPLY);
	}
}

void CalculatorScreen::divideClicked(lv_event_t *e)
{
	lv_obj_t *const key = lv_event_get_current_target_obj(e);
	CalculatorScreen *const screen = static_cast<CalculatorScreen *>(
		lv_obj_get_user_data(lv_obj_get_parent(key))
	);
	if (screen != nullptr) {
		screen->pressOperation(Operation::DIVIDE);
	}
}

void CalculatorScreen::equalsClicked(lv_event_t *e)
{
	lv_obj_t *const key = lv_event_get_current_target_obj(e);
	CalculatorScreen *const screen = static_cast<CalculatorScreen *>(
		lv_obj_get_user_data(lv_obj_get_parent(key))
	);
	if (screen != nullptr) {
		screen->pressEquals();
	}
}

void CalculatorScreen::backspaceClicked(lv_event_t *e)
{
	lv_obj_t *const key = lv_event_get_current_target_obj(e);
	CalculatorScreen *const screen = static_cast<CalculatorScreen *>(
		lv_obj_get_user_data(lv_obj_get_parent(key))
	);
	if (screen != nullptr) {
		screen->pressBackspace();
	}
}

void CalculatorScreen::backspaceLongPressed(lv_event_t *e)
{
	lv_obj_t *const key = lv_event_get_current_target_obj(e);
	CalculatorScreen *const screen = static_cast<CalculatorScreen *>(
		lv_obj_get_user_data(lv_obj_get_parent(key))
	);
	if (screen != nullptr) {
		screen->clear();
	}
}

void CalculatorScreen::drawBackspace(lv_event_t *e)
{
	lv_obj_t *const button = lv_event_get_current_target_obj(e);
	lv_layer_t *const layer = lv_event_get_layer(e);

	lv_area_t area;
	lv_obj_get_coords(button, &area);
	const int32_t cx = (area.x1 + area.x2) / 2;
	const int32_t cy = (area.y1 + area.y2) / 2;

	const lv_point_t points[CalculatorScreen::ARROW_POINTS_COUNT] = {
		{cx - CalculatorScreen::ARROW_TIP, cy},
		{cx - CalculatorScreen::ARROW_NECK, cy - CalculatorScreen::ARROW_HALF_HEIGHT},
		{cx + CalculatorScreen::ARROW_RIGHT, cy - CalculatorScreen::ARROW_HALF_HEIGHT},
		{cx + CalculatorScreen::ARROW_RIGHT, cy + CalculatorScreen::ARROW_HALF_HEIGHT},
		{cx - CalculatorScreen::ARROW_NECK, cy + CalculatorScreen::ARROW_HALF_HEIGHT},
		{cx - CalculatorScreen::ARROW_TIP, cy},
	};

	lv_draw_line_dsc_t dsc;
	lv_draw_line_dsc_init(&dsc);
	dsc.width = CalculatorScreen::LINE_WIDTH;
	dsc.round_start = 1;
	dsc.round_end = 1;
	dsc.color = lv_color_hex(COLOR_GREEN);

	for (uint8_t i = 0; i < CalculatorScreen::ARROW_POINTS_COUNT - 1; i++) {
		dsc.p1 = {
			static_cast<lv_value_precise_t>(points[i].x),
			static_cast<lv_value_precise_t>(points[i].y),
		};
		dsc.p2 = {
			static_cast<lv_value_precise_t>(points[i + 1].x),
			static_cast<lv_value_precise_t>(points[i + 1].y),
		};
		lv_draw_line(layer, &dsc);
	}

	CalculatorScreen::drawBackspaceCross(layer, &dsc, cx, cy);
}

void CalculatorScreen::drawBackspaceCross(
	lv_layer_t *const layer,
	lv_draw_line_dsc_t *const dsc,
	const int32_t cx,
	const int32_t cy
)
{
	const int32_t left = cx - CalculatorScreen::CROSS_LEFT;
	const int32_t right = cx + CalculatorScreen::CROSS_RIGHT;
	const int32_t top = cy - CalculatorScreen::CROSS_HALF_HEIGHT;
	const int32_t bottom = cy + CalculatorScreen::CROSS_HALF_HEIGHT;

	dsc->p1 = {static_cast<lv_value_precise_t>(left), static_cast<lv_value_precise_t>(top)};
	dsc->p2 = {static_cast<lv_value_precise_t>(right), static_cast<lv_value_precise_t>(bottom)};
	lv_draw_line(layer, dsc);

	dsc->p1 = {static_cast<lv_value_precise_t>(left), static_cast<lv_value_precise_t>(bottom)};
	dsc->p2 = {static_cast<lv_value_precise_t>(right), static_cast<lv_value_precise_t>(top)};
	lv_draw_line(layer, dsc);
}
