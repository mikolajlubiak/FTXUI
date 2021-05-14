#include <functional>  // for function
#include <memory>      // for shared_ptr

#include "ftxui/component/captured_mouse.hpp"  // for CapturedMouse
#include "ftxui/component/checkbox.hpp"
#include "ftxui/component/event.hpp"  // for Event, Event::Return
#include "ftxui/component/mouse.hpp"  // for Mouse, Mouse::Left, Mouse::Pressed
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive

namespace ftxui {

/// @brief Draw checkable element.
/// @param label The label of the checkbox.
/// @param checked Whether the checkbox is checked or not.
/// @ingroup component
/// @see CheckboxBase
///
/// ### Example
///
/// ```cpp
/// auto screen = ScreenInteractive::FitComponent();
/// std::wstring label = L"Make a sandwidth";
/// bool checked = false;
/// Component checkbox = Checkbox(&label, &checked);
/// screen.Loop(checkbox)
/// ```
///
/// ### Output
///
/// ```bash
/// ☐ Make a sandwitch
/// ```
Component Checkbox(ConstStringRef label, bool* checked) {
  return Make<CheckboxBase>(label, checked);
}

// static
CheckboxBase* CheckboxBase::From(Component component) {
  return static_cast<CheckboxBase*>(component.get());
}

CheckboxBase::CheckboxBase(ConstStringRef label, bool* state)
    : label_(label), state_(state) {}

Element CheckboxBase::Render() {
  bool is_focused = Focused();
  auto style = is_focused ? focused_style : unfocused_style;
  auto focus_management = is_focused ? focus : *state_ ? select : nothing;
  return hbox(text(*state_ ? checked : unchecked),
              text(*label_) | style | focus_management) |
         reflect(box_);
}

bool CheckboxBase::OnEvent(Event event) {
  if (event.is_mouse())
    return OnMouseEvent(event);

  if (event == Event::Character(' ') || event == Event::Return) {
    *state_ = !*state_;
    on_change();
    return true;
  }
  return false;
}

bool CheckboxBase::OnMouseEvent(Event event) {
  if (!CaptureMouse(event))
    return false;
  if (!box_.Contain(event.mouse().x, event.mouse().y))
    return false;

  TakeFocus();

  if (event.mouse().button == Mouse::Left &&
      event.mouse().motion == Mouse::Pressed) {
    *state_ = !*state_;
    on_change();
    return true;
  }

  return false;
}

}  // namespace ftxui

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
