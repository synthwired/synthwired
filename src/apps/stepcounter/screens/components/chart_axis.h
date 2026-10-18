#pragma once

#include "core/ui/components/component.h"

class StepCounterChartAxis : public Component<StepCounterChartAxis>
{

	public:
		static constexpr const char *NAME = "step_counter_chart_axis";
		static constexpr const char *XML = R"xml(
<component>
	<styles>
		<style name="axis" bg_opa="100%" bg_color="#color_green_dark" border_width="0" pad_all="0" radius="0"/>
		<style name="grid" bg_opa="100%" bg_color="#color_muted" border_width="0" pad_all="0" radius="0"/>
		<style name="hour" text_align="center" text_font="digital_20" text_color="#color_muted"/>
	</styles>
	<view extends="lv_obj" width="100%" height="100%" clickable="false" scrollable="false" overflow_visible="true">
		<style name="container"/>

		<lv_obj x="25%" y="14%" width="1" height="64%" clickable="false" scrollable="false">
			<style name="grid"/>
		</lv_obj>
		<lv_obj x="50%" y="14%" width="1" height="64%" clickable="false" scrollable="false">
			<style name="grid"/>
		</lv_obj>
		<lv_obj x="75%" y="14%" width="1" height="64%" clickable="false" scrollable="false">
			<style name="grid"/>
		</lv_obj>

		<lv_obj y="78%" width="100%" height="1" clickable="false" scrollable="false">
			<style name="axis"/>
		</lv_obj>

		<lv_obj x="0%" y="78%" width="2" height="6" style_translate_x="-1" clickable="false" scrollable="false">
			<style name="grid"/>
		</lv_obj>
		<lv_obj x="25%" y="78%" width="2" height="6" style_translate_x="-1" clickable="false" scrollable="false">
			<style name="grid"/>
		</lv_obj>
		<lv_obj x="50%" y="78%" width="2" height="6" style_translate_x="-1" clickable="false" scrollable="false">
			<style name="grid"/>
		</lv_obj>
		<lv_obj x="75%" y="78%" width="2" height="6" style_translate_x="-1" clickable="false" scrollable="false">
			<style name="grid"/>
		</lv_obj>
		<lv_obj x="100%" y="78%" width="2" height="6" style_translate_x="-1" clickable="false" scrollable="false">
			<style name="grid"/>
		</lv_obj>

		<lv_label text="00" x="0%" y="83%" width="24" style_text_align="left">
			<style name="hour"/>
		</lv_label>
		<lv_label text="06" x="25%" y="83%" width="24" style_translate_x="-12">
			<style name="hour"/>
		</lv_label>
		<lv_label text="12" x="50%" y="83%" width="24" style_translate_x="-12">
			<style name="hour"/>
		</lv_label>
		<lv_label text="18" x="75%" y="83%" width="24" style_translate_x="-12">
			<style name="hour"/>
		</lv_label>
		<lv_label text="24" x="100%" y="83%" width="24" style_translate_x="-24" style_text_align="right">
			<style name="hour"/>
		</lv_label>
	</view>
</component>
)xml";
};
