#pragma once

#include <cstddef>
#include <cstdint>
#include <lvgl.h>
#include "core/xml_screen.h"

class CalculatorScreen : public XmlScreen<CalculatorScreen>
{

	public:
		static constexpr const char *NAME = "calculator_index";
		static const char *const XML;

		static void registerCallbacks();
		void render() override;
		const char *getTitle() override;
		bool isFramed() override;
		bool hasBackground() override;

	protected:
		enum class Operation : uint8_t
		{
			NONE,
			ADD,
			SUBTRACT,
			MULTIPLY,
			DIVIDE,
		};

		static constexpr size_t INPUT_SIZE = 16;
		static constexpr uint8_t MAX_DIGITS = 10;
		static constexpr size_t LARGE_FONT_MAX_LENGTH = 8;
		static constexpr double MAX_VALUE = 1e10;
		static constexpr uint8_t OPERATORS_COUNT = 4;
		static constexpr int32_t SIGN_GAP = 2;
		static constexpr const char *OPERATION_SYMBOLS[OPERATORS_COUNT + 1] = {"", "+", "-", "×", "÷"};
		static constexpr int32_t ARROW_TIP = 13;
		static constexpr int32_t ARROW_NECK = 6;
		static constexpr int32_t ARROW_RIGHT = 12;
		static constexpr int32_t ARROW_HALF_HEIGHT = 7;
		static constexpr int32_t CROSS_LEFT = 1;
		static constexpr int32_t CROSS_RIGHT = 6;
		static constexpr int32_t CROSS_HALF_HEIGHT = 3;
		static constexpr int32_t LINE_WIDTH = 2;
		static constexpr uint8_t ARROW_POINTS_COUNT = 6;

		char input[INPUT_SIZE] = "0";
		double saved = 0.0;
		Operation operation = Operation::NONE;
		bool replaceInput = false;
		bool error = false;
		lv_obj_t *numberLabel = nullptr;
		lv_obj_t *signLabel = nullptr;
		lv_obj_t *pendingLabel = nullptr;
		lv_obj_t *operatorKeys[OPERATORS_COUNT] = {};

		void pressDigit(char digit);
		void pressOperation(Operation next);
		void pressEquals();
		void pressBackspace();
		void clear();
		void calculate();
		void updateDisplay();
		void updateNumber();
		void updatePending();

		static uint8_t getDigitsCount(const char *input);
		static bool formatNumber(
			double value,
			char *buffer,
			size_t size
		);
		static void digitClicked(lv_event_t *e);
		static void addClicked(lv_event_t *e);
		static void subtractClicked(lv_event_t *e);
		static void multiplyClicked(lv_event_t *e);
		static void divideClicked(lv_event_t *e);
		static void equalsClicked(lv_event_t *e);
		static void backspaceClicked(lv_event_t *e);
		static void backspaceLongPressed(lv_event_t *e);
		static void drawBackspace(lv_event_t *e);
		static void drawBackspaceCross(
			lv_layer_t *layer,
			lv_draw_line_dsc_t *dsc,
			int32_t cx,
			int32_t cy
		);
};
