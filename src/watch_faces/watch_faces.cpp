#include "watch_faces.h"

#include "core/watchface_registry.h"
#include "circuit.h"
#include "knight_rider.h"
#include "lcars.h"
#include "neon_circles.h"
#include "sunset.h"
#include "synthwired.h"
#include "synthwired_analog.h"
#include "target_lock.h"

void register_watchfaces()
{
	WatchfaceRegistry *const registry = WatchfaceRegistry::getInstance();

	static SynthWired synth_wired;
	registry->registerWatchface(&synth_wired);

	static Circuit circuit;
	registry->registerWatchface(&circuit);

	static NeonCircles neon_circles;
	registry->registerWatchface(&neon_circles);

	static SynthWiredAnalog synth_wired_analog;
	registry->registerWatchface(&synth_wired_analog);

	static Sunset sunset;
	registry->registerWatchface(&sunset);

	static TargetLock target_lock;
	registry->registerWatchface(&target_lock);

	static KnightRider knight_rider;
	registry->registerWatchface(&knight_rider);

	static Lcars lcars;
	registry->registerWatchface(&lcars);
}
