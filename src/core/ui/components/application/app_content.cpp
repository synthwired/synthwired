#include "app_content.h"

#include "core/ui/components/utils/chamfer_frame.h"

void AppContent::registerCallbacks()
{
	lv_xml_register_event_cb(nullptr, "draw_app_content", AppContent::drawFrame);
}

void AppContent::drawFrame(lv_event_t *e)
{
	lv_obj_t *const obj = lv_event_get_current_target_obj(e);
	lv_layer_t *const layer = lv_event_get_layer(e);

	ChamferFrame::drawCorners(obj, layer);
	ChamferFrame::drawOutline(obj, layer);
}
