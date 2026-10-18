#pragma once

class Application {

	public:
		virtual ~Application() = default;
		virtual void render() {};
		virtual void everySecond() {};
		virtual void everyMinute() {};
		virtual void swipeLeft() {};
		virtual void swipeRight() {};
		virtual void swipeUp() {}
		virtual void swipeDown() {};
		virtual void buttonPressed() {};

};
