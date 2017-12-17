#ifndef __j1CREDITS_H__
#define __j1CREDITS_H__

#include "j1Module.h"
#include "p2DynArray.h"

#define BUTTON_HOVER_OFFSET 6
#define BUTTON_PUSH_OFFSET 3

class UIButton;
class UILabel;
class UIImage;
class UISlider;
class UICheckBox;

class j1Credits : public j1Module
{
public:

	j1Credits();

	// Destructor
	virtual ~j1Credits();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	void OnUITrigger(UIElement* elementTriggered, UI_State ui_state);

	iPoint GetPointToCenter(int w_to_center, int h_to_center, int w_ref, int h_ref);

	void ResetTextToLabels();

public:
	bool move_camera = false;
	bool capped_checkbox = false;

private:

	p2DynArray<UIButton*> buttons;
	p2DynArray<UILabel*> labels;
	p2DynArray<UIElement*> settings_elements;

};


#endif // __j1MAINMENU_H__