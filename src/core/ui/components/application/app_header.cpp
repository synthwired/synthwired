#include "app_header.h"

#include "core/ui/components/utils/chamfer_frame.h"

void AppHeader::registerCallbacks()
{
	lv_xml_register_event_cb(nullptr, "draw_header_corners", AppHeader::drawCorners);
}

void AppHeader::drawCorners(lv_event_t *e)
{
	lv_obj_t *const obj = lv_event_get_current_target_obj(e);
	lv_layer_t *const layer = lv_event_get_layer(e);

	lv_area_t area;
	lv_obj_get_coords(obj, &area);

	ChamferFrame::drawCorner(
		layer,
		area.x1 - 1,
		area.y2 + 1,
		1,
		-1,
		AppHeader::CUT
	);
	ChamferFrame::drawCorner(
		layer,
		area.x2 + 1,
		area.y2 + 1,
		-1,
		-1,
		AppHeader::CUT
	);
}
