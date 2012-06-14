#include "CMenu.h"

//Menu
Menu::Menu(){
	SDL_Color tmp_act = {255, 128, 128, 0};
	SDL_Color tmp_pass = {0, 0, 0, 0};

	cursor = NULL;
	col_active = tmp_act;
	col_passive = tmp_pass;

	activeLine = 0;
}

Menu::~Menu(){
	if (cursor != NULL){
		SDL_FreeSurface(cursor);
	}
}

void Menu::setCursor(const char *cur_path){
	cursor = loadImage(cur_path, true);
}

void Menu::setActiveColor(SDL_Color color){
	col_active = color;
}

void Menu::setPassiveColor(SDL_Color color){
	col_passive = color;
}

SDL_Color Menu::getActiveColor(){
	return col_active;
}

SDL_Color Menu::getPassiveColor(){
	return col_passive;
}

void Menu::addLine(string newLine){
	lines.push_back(newLine);
}

void Menu::changeLine(int n){
	if (lines.size() != 0){		
		if (n != -1){
			activeLine = n % lines.size();
		} else{
			activeLine = lines.size() -1;
		}
	} else{
		return;
	}

	it_lines = lines.begin();
	for (int i = 0; i < activeLine; i++){
		it_lines++;
	}
}

void Menu::deleteLines(){
	lines.clear();
}

int Menu::getActiveLine(){
	return activeLine;
}

string Menu::getActiveLineAsString(){
	return *it_lines;
}

void Menu::setActiveLine(string new_line){
	if (it_lines != lines.end()){
		(*it_lines) = new_line;
	}
}

//PageInMenu
PageInMenu::PageInMenu(): Menu(){
	bg = NULL;
	font = NULL;
}

PageInMenu::PageInMenu(SDL_Surface& new_bg, TTF_Font& new_font): Menu(){
	bg = &new_bg;
	font = &new_font;
}

PageInMenu::~PageInMenu(){
	if (bg != NULL){
		//uvolnovat bg nemusim, lebo to je referencia
		bg = NULL;
	}
	lines.clear();
}

void PageInMenu::setBackground(SDL_Surface &new_bg){
	bg = &new_bg;
}

SDL_Surface& PageInMenu::getBackground(){
	return *bg;
}

void PageInMenu::setFont(TTF_Font &new_font){
	font = &new_font;
}

TTF_Font& PageInMenu::getFont(){
	return *font;
}

void PageInMenu::changeLine(unsigned int x, unsigned int y){	
	int height_line = 0;
	TTF_SizeText(font, "", NULL, &height_line); //zistenie vysky riadku
	if (y > ((RES_Y / 2) - (height_line*lines.size() / 2))
     && y < ((RES_Y / 2) + (height_line*lines.size() / 2))){
		 Menu::changeLine((y - (RES_Y / 2) + (height_line*lines.size() / 2)) / height_line);
	}
}

CompleteMenu::CompleteMenu(): no_actualPage(1){

}

CompleteMenu::~CompleteMenu(){
	pages.clear(); //zavolenie destruktorov pre kazdu stranu a zmazanie vectora
}

unsigned int CompleteMenu::getActualPage(){
	return no_actualPage;
}

void CompleteMenu::addPage(PageInMenu new_page){
	pages.push_back(new_page);
	it_actualPage = pages.begin();
	no_actualPage = 1;
}

void CompleteMenu::changeActualPage(unsigned int n){
	if (n > pages.size() || n < 1){
		return;
	}
	for (no_actualPage = 1, it_actualPage = pages.begin(); no_actualPage < n; no_actualPage++, it_actualPage++);
}

void CompleteMenu::draw(SDL_Surface *src){
	SDL_BlitSurface(&it_actualPage->getBackground(), NULL, src, NULL); //prilepenie pozadia na graficku plochu

	//ak nemame riadky
	if (pages.empty()){
		return;
	}

	//sem sa nakopiruje text z kazdeho riadku a ten sa potom vykresli
	SDL_Surface *line = NULL;

	SDL_Rect pos; //pozicia poloziek
	SDL_Color col; //farba riadku

	//prejdenie vsetkych riadkov na aktualnej strane a ich vykreslenie
	for (vector<string>::iterator it_actualLine = it_actualPage->lines.begin(); it_actualLine != it_actualPage->lines.end(); it_actualLine++){
		//najdenie aktivneho riadku - nastavenie aktivnej farby
		if (it_actualLine - it_actualPage->lines.begin() == it_actualPage->getActiveLine()){
			col = it_actualPage->getActiveColor();
		} else{
			col = it_actualPage->getPassiveColor();
		}
	
		line = TTF_RenderText_Blended(&it_actualPage->getFont(), it_actualLine->c_str(), col); //prerobenie riadka na SDL_Surface
		pos.x = (src->w >> 1) - (line->w >> 1); //vycentrovanie riadku na sirke obrazovky
		pos.y = (src->h >> 1) - (line->h*it_actualPage->lines.size() >> 1) + (it_actualLine - it_actualPage->lines.begin())*(line->h); //vycentrovanie riadku na vyske obrazkovy
		SDL_BlitSurface(line, NULL, src, &pos); //prilepenie riadku
		SDL_FreeSurface(line); //uvolnenie, uz ho nepotrebujeme
	}
}

bool menuEventChangePage(CompleteMenu &m){
	switch(m.getActualPage()){
		//Hlavna obrazovka
		case 1:
			switch(m.it_actualPage->getActiveLine()){
				//Start Game
				case 0:
					menuMode = false;
					*m.it_actualPage->lines.begin() = "Continue";
					break;
				//Options
				case 1:
					m.changeActualPage(2);
					break;
				//About
				case 2:
					m.changeActualPage(3);
					m.it_actualPage->Menu::changeLine(7);
					break;
				//Exit
				case 3:
					return false;
					break;
			}
			break;
		//Options
		case 2:
			switch(m.it_actualPage->getActiveLine()){
				//Graphics
				case 0:
					m.it_actualPage->Menu::changeLine(0);
					m.changeActualPage(4);
					break;
				//Sound
				case 1:
					m.it_actualPage->Menu::changeLine(0);
					m.changeActualPage(7);
					break;
				//Keyboard
				case 2:
					break;
				//Return
				case 4:
					m.it_actualPage->Menu::changeLine(0);
					m.changeActualPage(1);
					break;
			}
			break;
		//About
		case 3:
			m.changeActualPage(1);
			break;
		//Graphics
		case 4:
			switch(m.it_actualPage->getActiveLine()){
				//Resolution
				case 0:
					m.it_actualPage->Menu::changeLine(0);
					m.changeActualPage(5);
					break;
				//Color Depth
				case 1:
					m.it_actualPage->Menu::changeLine(0);
					m.changeActualPage(6);
					break;
				//Fullscreen
				case 2:
					fullscreenMode = !fullscreenMode;
					ConfigGame();
					break;
				//Return
				case 4:
					m.it_actualPage->Menu::changeLine(0);
					m.changeActualPage(2);
					break;
			}
			break;
		//Resolution
		case 5:
			//ak Return
			if (m.it_actualPage->getActiveLine() == m.it_actualPage->lines.size() - 1){
			} else{
				//ak nejake rozlisenie
				stringstream line;
				line << m.it_actualPage->getActiveLineAsString();
				if (line.str() != " "){
					string w; string h;

					getline(line, w, 'x');
					getline(line, h);

					RES_X = atoi(w.c_str());
					RES_Y = atoi(h.c_str());
				}
			}
			m.it_actualPage->Menu::changeLine(0);
			m.changeActualPage(4);
			break;
		//Color Depth
		case 6:
			switch(m.it_actualPage->getActiveLine()){
				//16
				case 0:
					BITDEPTH = 16;
					break;
				//24
				case 1:
					BITDEPTH = 24;
					break;
				//32
				case 2:
					BITDEPTH = 32;
					break;
				//Same as in OS
				case 3:
					BITDEPTH = 0;
					break;
			}
			m.it_actualPage->Menu::changeLine(0);
			m.changeActualPage(4);
			break;
		//Sound
		case 7:
			string text;
			switch(m.it_actualPage->getActiveLine()){
				//Game music
				case 0:
					if (musicStatus)
						Mix_FadeOutChannel(0, 25);
					else {
						Mix_PlayChannel(0, overworld, -1);
						Mix_Pause(0);
					}
					musicStatus = !musicStatus;
					text = "Main music - ";
					text += musicStatus ? "on" : "off";
					m.it_actualPage->setActiveLine(text);
					break;
				//Sound effects
				case 1:
					effectsStatus = !effectsStatus;
					text = "Sound effects - ";
					text += effectsStatus ? "on" : "off";
					m.it_actualPage->setActiveLine(text);
					break;
				//Return
				case 3:
					m.it_actualPage->Menu::changeLine(0);
					m.changeActualPage(2);
					break;
			}
			break;
	}
	m.it_actualPage->changeLine(mouse_x, mouse_y);
	return true;
}

void menuEventKeyUp(CompleteMenu &m){
	//zmen riadok
	m.it_actualPage->Menu::changeLine(m.it_actualPage->getActiveLine() - 1);
	//ak som na prazdnom riadku, zmen o 1 este raz
	if (m.getActualPage() == 2 && m.it_actualPage->getActiveLine() == 3
	 || m.getActualPage() == 3 && m.it_actualPage->getActiveLine() == 2
	 || m.getActualPage() == 3 && m.it_actualPage->getActiveLine() == 6
	 || m.getActualPage() == 4 && m.it_actualPage->getActiveLine() == 3
	 || m.getActualPage() == 5 && m.it_actualPage->getActiveLine() == (m.it_actualPage->lines.size() - 2)
	 || m.getActualPage() == 7 && m.it_actualPage->getActiveLine() == 2){
			m.it_actualPage->Menu::changeLine(m.it_actualPage->getActiveLine() - 1);
	}
}

void menuEventKeyDown(CompleteMenu &m){
	//zmen riadok
	m.it_actualPage->Menu::changeLine(m.it_actualPage->getActiveLine() + 1);
	//ak som na prazdnom riadku, zmen o 1 este raz
	if (m.getActualPage() == 2 && m.it_actualPage->getActiveLine() == 3
	 || m.getActualPage() == 3 && m.it_actualPage->getActiveLine() == 2
	 || m.getActualPage() == 3 && m.it_actualPage->getActiveLine() == 6
	 || m.getActualPage() == 4 && m.it_actualPage->getActiveLine() == 3
	 || m.getActualPage() == 5 && m.it_actualPage->getActiveLine() == (m.it_actualPage->lines.size() - 2)
	 || m.getActualPage() == 7 && m.it_actualPage->getActiveLine() == 2){
			m.it_actualPage->Menu::changeLine(m.it_actualPage->getActiveLine() + 1);
	}
}

bool menuEventKeyESC(CompleteMenu &m){
	switch(m.getActualPage()){
		//Hlavna obrazovka
		case 1:
			return false;
			break;
		//Options
		case 2:
			m.it_actualPage->Menu::changeLine(0);
			m.changeActualPage(1);
			break;
		//About
		case 3:
			m.it_actualPage->Menu::changeLine(0);
			m.changeActualPage(1);
			break;
		//Graphics
		case 4:
			//ak bola vykonana zmena v nastaveniach, tak vytvor nove okno s danymi nastaveniami
			m.it_actualPage->Menu::changeLine(0);
			m.changeActualPage(2);
			break;
		//Resolution
		case 5:
			m.it_actualPage->Menu::changeLine(0);
			m.changeActualPage(4);
			break;
		//Color Depth
		case 6:
			m.it_actualPage->Menu::changeLine(0);
			m.changeActualPage(4);
			break;
		case 7:
			m.it_actualPage->Menu::changeLine(0);
			m.changeActualPage(2);
			break;
	}
	m.it_actualPage->changeLine(mouse_x, mouse_y);
	return true;
}

bool processEventsInMenu(CompleteMenu &m){
	if (!menuMode){
		return true;
	}
	
	SDL_Event event;
	//spracovanie vsetkych udalosti na fronte, ak nic tam neni tak sa cyklus nevykona
	while(SDL_PollEvent(&event)){
		switch(event.type){
			//bola stlacena klavesa
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_UP:
						menuEventKeyUp(m);
						break;
					case SDLK_DOWN:
						menuEventKeyDown(m);
						break;
					case SDLK_RETURN:
						if (!menuEventChangePage(m)){
							return false;
						}
						break;
					case SDLK_ESCAPE: //ak bolo stlacene ESC
						if (!menuEventKeyESC(m)){
							return false;
						}
						break;
				}
				break;
			//bola pohnuta myska
			case SDL_MOUSEMOTION:				
				mouse_x = event.motion.x;
				mouse_y = event.motion.y;
				m.it_actualPage->changeLine(mouse_x, mouse_y);
				break;
			//stlacena myska
			case SDL_MOUSEBUTTONDOWN:
				//co bolo stlacene
				switch(event.button.button){
					case SDL_BUTTON_LEFT:
						if (!menuEventChangePage(m)){
							return false;
						}
				}
				break; 
			//poziadavka na ukoncenie aplikacie
			case SDL_QUIT:
				return false;
				break;
		}

	}
	return true;
}