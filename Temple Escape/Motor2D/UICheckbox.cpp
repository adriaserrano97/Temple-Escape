#include "j1App.h"
#include "UICheckBox.h"
#include "p2Log.h"
#include "j1Audio.h"


UICheckBox::UICheckBox(int x, int y, UI_Type type, SDL_Rect cb_tick_normal, SDL_Rect cb_no_tick_normal, SDL_Rect cb_tick_pressed, SDL_Rect cb_no_tick_pressed, SDL_Rect cb_tick_focus, SDL_Rect cb_no_tick_focus, j1Module* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;

	//TODO : this looks ugly but it seems the only way :'c
	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadConfig(config_file); //todo: make a method to get the root without passing the xml_document
	node = &node->child("ui").child("button").child("fx");

	//read animation from node

	pressed_fx = (App->audio->LoadFx(node->attribute("pressed").as_string()));
	released_fx = (App->audio->LoadFx(node->attribute("released").as_string()));
	hover_fx = (App->audio->LoadFx(node->attribute("hover").as_string()));

	/*this->cb_tick = cb_tick;
	this->cb_no_tick = cb_no_tick;*/
	this->cb_tick_focus = cb_tick_focus;
	this->cb_no_tick_focus = cb_no_tick_focus;
	//current_rect = cb_tick;

	LOG("UICheckBox created in x:%i, y:%i", x, y);
}

UICheckBox::~UICheckBox()
{
	LOG("Freeing the CheckBox");

	LOG("Unloading button sound fx");
	App->audio->UnLoadFx(pressed_fx);
	App->audio->UnLoadFx(released_fx);

}

void UICheckBox::UpdateCheckBoxWithSelfRect(SDL_Rect self_rect)
{
	/*if (self_rect.x == btn_pressed.x  && self_rect.y == btn_pressed.y && self_rect.w == btn_pressed.w && self_rect.h == btn_pressed.h)
		App->audio->PlayFx(pressed_fx);
	else if (self_rect.x == btn_normal.x  && self_rect.y == btn_normal.y && self_rect.w == btn_normal.w && self_rect.h == btn_normal.h && last_state != STATE_MOUSE_ENTER)
		App->audio->PlayFx(released_fx);
	else if (self_rect.x == btn_focused.x  && self_rect.y == btn_focused.y && self_rect.w == btn_focused.w && self_rect.h == btn_focused.h && last_state != STATE_LEFT_MOUSE_RELEASED)
		App->audio->PlayFx(hover_fx);*/

	current_rect = self_rect;
}