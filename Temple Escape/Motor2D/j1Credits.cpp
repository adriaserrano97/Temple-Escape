#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1FadeToBlack.h"
#include "j1MainMenu.h"
#include "j1Credits.h"
#include "j1UI.h"
#include "j1Language.h"

#include "UIImage.h"
#include "UIButton.h"

#include "j1Entities.h"

j1Credits::j1Credits() : j1Module()
{
	name.create("credits");
}

// Destructor
j1Credits::~j1Credits()
{}

// Called before render is available

bool j1Credits::Awake()
{

	LOG("Loading j1Credits");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Credits::Start()
{
	bool ret = true;

	if (!App->audio->PlayMusic("audio/music/Visager_End_Credits.ogg"))
	{
		//ret = false;
		LOG("Error playing music in j1Credits Start");
	}
	p2SString licence;
	licence.create("MIT License                                      Copyright(c) 2017                                         Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the ''Software''), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :                                                  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.                                                          THE SOFTWARE IS PROVIDED ''AS IS'', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");
	
	uint w, h;
	App->win->GetWindowSize(w, h);
	
	licence_lbl = (UILabel*)App->ui->AddUILabel(50, h / App->win->GetScale(), licence, WHITE, 10, 200);
	licence_lbl->interactable = false;
	labels.PushBack(licence_lbl);

	p2SString the_team;
	the_team.create("THE TEAM");

	UILabel* team_lbl = (UILabel*)App->ui->AddUILabel(50, h / App->win->GetScale() + licence_lbl->GetRect().h, the_team, WHITE, 10);
	team_lbl->interactable = false;
	labels.PushBack(team_lbl);

	p2SString hold;
	hold.create("Hold space to skip");

	hold_lbl = (UILabel*)App->ui->AddUILabel(0,5, hold, WHITE, 10);
	hold_lbl->SetLocalPosition(w / App->win->GetScale() / 2 - hold_lbl->GetRect().w / 2, hold_lbl->GetLocalPosition().y);
	hold_lbl->interactable = false;

	team_photo = (UIImage*)App->ui->AddUIImage(0, h / App->win->GetScale() + licence_lbl->GetRect().h + team_lbl->GetRect().h, { 0,487,336,252 });
	team_photo->interactable = false;

	return ret;
}

// Called each loop iteration
bool j1Credits::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Credits::Update(float dt)
{

	int holding_space = 1;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		holding_space = 4;
	}

	for (int i = 0; i < labels.Count(); i++)
	{
		labels[i]->SetLocalPosition(labels[i]->GetLocalPosition().x, labels[i]->GetLocalPosition().y - scroll_speed * holding_space);
	}

	team_photo->SetLocalPosition(team_photo->GetLocalPosition().x, team_photo->GetLocalPosition().y - scroll_speed * holding_space);

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN&& App->fadeToBlack->FadeIsOver())
	{
		App->fadeToBlack->FadeToBlackBetweenModules(this, App->main_menu, 1.0f);
	}

	if (licence_lbl->GetLocalPosition().y - 50 < 0) {
		hold_lbl->invisible = true;
	}

	return true;
}


// Called each loop iteration
bool j1Credits::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool j1Credits::CleanUp()
{
	LOG("Freeing j1Credits");
	App->ui->DeleteAllUIElements();

	return true;
}

bool j1Credits::Load(pugi::xml_node& load)
{
	bool ret = true;

	
	return ret;
}

bool j1Credits::Save(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}

void j1Credits::OnUITrigger(UIElement* elementTriggered, UI_State ui_state) {
	if (elementTriggered->type == BUTTON)
	{
		UIButton* tmpBtn = (UIButton*)elementTriggered;

		switch (ui_state)
		{
		case STATE_LEFT_MOUSE_RELEASED:
		case STATE_NORMAL:
		{
			tmpBtn->SetLocalPosition(tmpBtn->GetLocalPosition().x, tmpBtn->GetLocalPosition().y - BUTTON_PUSH_OFFSET);
			if (tmpBtn->last_state == STATE_LEFT_MOUSE_PRESSED &&  App->fadeToBlack->FadeIsOver())
			{
				

			}
			tmpBtn->UpdateButtonWithSelfRect(tmpBtn->btn_normal);
			break;
		}
		case STATE_MOUSE_ENTER:
		{
			for (int i = 0; i < buttons.Count(); i++)
			{
				if (buttons[i]->current_state == STATE_FOCUSED)
				{
					buttons[i]->current_state = STATE_NORMAL;
					tmpBtn->UpdateButtonWithSelfRect(tmpBtn->btn_normal);
					break;
				}
			}

			tmpBtn->UpdateButtonWithSelfRect(tmpBtn->btn_focused);
			if (tmpBtn->button_lbl != nullptr)
				tmpBtn->button_lbl->SetLocalPosition(tmpBtn->button_lbl->GetLocalPosition().x + BUTTON_HOVER_OFFSET, tmpBtn->button_lbl->GetLocalPosition().y + BUTTON_HOVER_OFFSET);
			tmpBtn->SetLocalPosition(tmpBtn->GetLocalPosition().x - BUTTON_HOVER_OFFSET, tmpBtn->GetLocalPosition().y - BUTTON_HOVER_OFFSET);
			break;
		}
		case STATE_MOUSE_LEAVE:
			tmpBtn->UpdateButtonWithSelfRect(tmpBtn->btn_normal);
			if (tmpBtn->last_state == STATE_LEFT_MOUSE_PRESSED)
			{
				tmpBtn->SetLocalPosition(tmpBtn->GetLocalPosition().x, tmpBtn->GetLocalPosition().y - BUTTON_PUSH_OFFSET);
			}
			else
			{
				if (tmpBtn->button_lbl != nullptr)
					tmpBtn->button_lbl->SetLocalPosition(tmpBtn->button_lbl->GetLocalPosition().x - BUTTON_HOVER_OFFSET, tmpBtn->button_lbl->GetLocalPosition().y - BUTTON_HOVER_OFFSET);
				tmpBtn->SetLocalPosition(tmpBtn->GetLocalPosition().x + BUTTON_HOVER_OFFSET, tmpBtn->GetLocalPosition().y + BUTTON_HOVER_OFFSET);
			}
			break;
		case STATE_LEFT_MOUSE_PRESSED:
			tmpBtn->UpdateButtonWithSelfRect(tmpBtn->btn_pressed);
			if (tmpBtn->button_lbl != nullptr)
				tmpBtn->button_lbl->SetLocalPosition(tmpBtn->button_lbl->GetLocalPosition().x - BUTTON_HOVER_OFFSET, tmpBtn->button_lbl->GetLocalPosition().y - BUTTON_HOVER_OFFSET);
			tmpBtn->SetLocalPosition(tmpBtn->GetLocalPosition().x + BUTTON_HOVER_OFFSET, tmpBtn->GetLocalPosition().y + BUTTON_HOVER_OFFSET + BUTTON_PUSH_OFFSET);
			break;
		case STATE_NO_DEF:
			break;
		default:
			break;
		}
	}
}
