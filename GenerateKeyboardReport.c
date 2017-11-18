#include "GenerateKeyboardReport.h"
#include "LayoutState.h"
#include "Keymap.h"
#include <LUFA/Drivers/USB/USB.h>

void Add_KeyCode_to_USB_KeyboardReport_Data(USB_KeyboardReport_Data_t* KeyboardReport, uint16_t KeyCode)
{
  switch(KeyCode){
    case HID_KEYBOARD_SC_LEFT_CONTROL:
      KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_LEFTCTRL;
      break;
    case HID_KEYBOARD_SC_LEFT_SHIFT:
      KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_LEFTSHIFT;
      break;
    case HID_KEYBOARD_SC_LEFT_ALT:
      KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_LEFTALT;
      break;
    case HID_KEYBOARD_SC_LEFT_GUI:
      KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_LEFTGUI;
      break;
    case HID_KEYBOARD_SC_RIGHT_CONTROL:
      KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_RIGHTCTRL;
      break;
    case HID_KEYBOARD_SC_RIGHT_SHIFT:
      KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_RIGHTSHIFT;
      break;
    case HID_KEYBOARD_SC_RIGHT_ALT:
      KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_RIGHTALT;
      break;
    case HID_KEYBOARD_SC_RIGHT_GUI:
      KeyboardReport->Modifier |= HID_KEYBOARD_MODIFIER_RIGHTGUI;
      break;
    default:
      for(uint8_t i=0 ; i < 6 ; i++)
        if(KeyboardReport->KeyCode[i] == KeyCode)
          goto end;
      for(uint8_t i=0 ; i < 6 ; i++){
        if(KeyboardReport->KeyCode[i] == 0){
          KeyboardReport->KeyCode[i] = KeyCode;
          break;
        }
      }
      end:
      break;
  }
}

void GenerateKeyboardReport(struct Key key, void *data)
{
  USB_KeyboardReport_Data_t* KeyboardReport;
  uint16_t key_value;
  uint8_t layout;

  KeyboardReport = (USB_KeyboardReport_Data_t*)data;

  for(uint8_t i=0 ; i < LAYER_COUNT ; i++) {
    if(!LayoutState_Get(i))
      continue;

    key_value = pgm_read_word(&(keymaps[i][key.row][key.column]));

    switch(GET_KEY_FN(key_value)){
      case KEY_FN_REG:
        if(key.state)
           Add_KeyCode_to_USB_KeyboardReport_Data(KeyboardReport, GET_KEY_CODE(key_value));
        goto finish;
        break;
      case KEY_FN_NONE:
        goto finish;
        break;
      case KEY_FN_PASS:
        break;
      case KEY_FN_MACRO:
        // FIXME warning: function with qualified void return type called
        keymap_macros[GET_MACRO(key_value)](key);
        goto finish;
        break;
      case KEY_FN_LAYOUT:
        layout = GET_LAYOUT(key_value);
        if(key.just_pressed) {
          for(uint8_t l=0; l < LAYOUT_LAYERS_COUNT ; l++) {
            if(keymap_layout_layers[l] == layout)
              LayoutState_Set(layout, 1);
            else
              LayoutState_Set(keymap_layout_layers[l], 0);
          }
        }
        goto finish;
        break;
    }
  }
  finish: ;
}
