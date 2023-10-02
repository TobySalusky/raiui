# RAIUI
### "Resource Aquisition is User Interface"
###### Using C++'s RAII (*Resource Aquisition is Initialization*) to write better immediate-mode, HTML/CSS-inspired UI code.

RAIUI leverages variable scope to emulate the tree structure you get with HTML DOMs and component frameworks with minimal code.

```c++
// all following elements will be inside container, until it goes out of scope
Div container ("my-css-style");

// use components in std::optional for conditional rendering!
if (auto tooltip = DelayTooltip(container)) {
  Text("You hovered me!", "dark-tooltip");
}

// create a span to contain/layout the subsequent buttons, with an inline style
Span buttonContainer (Style { .justifyContent = AlignType::SpaceBetween });

// dynamically render multiple buttons using a for loop -- no callbacks needed
for (auto action : std::vector<std::string> { "Continue", "Cancel", "Help" }) {
  if (ButtonPress(action, "cool-button")) {
    std::cout << "You pressed " << action << std::endl;
  }
}
```

RAIUI maintains all the joy of writing immediate-mode UI for real-time applications or games while allowing flexible customization with CSS styling.
> Note that only a small subset of CSS is currently implemented, [make a Feature Request](https://github.com/TobySalusky/raiui/issues) if your favorite styling utility is missing!



Or try out the *experimental* tailwind inspired styling primitives

```c++
Div tailwindDiv (w_fill & h[36] & bg_neutral_700
                        & hover/bg_neutral_600
                        & when[selected]/bg_blue_600);
```

---
## Backends
Currently RAIUI runs on Raylib, (more specifically, [raylib-cpp](https://github.com/RobLoach/raylib-cpp))

In the future, I hope to abstract RAIUI to be framework/rendering-backend agnostic.

--- 
Be warned ⚠️

RAIUI is far from stable and may change drastically as I explore this concept.

---
Feel free to suggest where you'd like to see this idea go by [making an Issue](https://github.com/TobySalusky/raiui/issues)
