## GPU profiling

The renderer also includes a simple GPU profiler. 
It is heavily based on this article:
http://reedbeta.com/blog/gpu-profiling-101/

You can switch on the profiling using the RenderSettings:

```c
ds::RenderSettings settings;
settings.useGPUProfiler = true;
```
Now the renderer will take care of staring and ending the profiling. It will also measure the time for the entire
frame. You can plugin in your profiling steps by calling measure:

```c
// do something
ds::submit(staticItem);
ds::gpu::measure(1);
```

Please note that 0 is reserved for the entire frame measurement.
Before you can get the data from the profiling you need to call:

```c
ds::gpu::waitForData();
```

This will collect the timings.

### API

The API uses the namespace ds::gpu. 

In order to profile you need to call measure after you habe submitted a draw call:
```c
void measure(int index);
```

The profiler needs to collect the data once you are and therefore you need to call:

```c
void waitForData();
```

This returns the duration for a certain measurement:

```c
float dt(int index);
```

This is the average durarion for a certain measurement. The average is calculated every 0.5 seconds:

```c
float dtAvg(int index);
```

This is the total time for an entire frame:

```c
float totalTime();
```

### Example

The Texture examples uses GPU profiling as an example. Here is a short example using the imgui:
 
```c
gui::start(ds::vec2(700, 750));
gui::begin("Timer", &state);
if (state == 1) {
	gui::Value("DrawTime", 1000.0f * ds::gpu::totalTime(), "%0.4f");
	gui::Value("DT1", 1000.0f * ds::gpu::dtAvg(1), "%0.4f");
	gui::Value("DT2", 1000.0f * ds::gpu::dtAvg(2), "%0.4f");
	gui::Value("DT3", 1000.0f * ds::gpu::dtAvg(3), "%0.4f");
}
gui::end();
```		

