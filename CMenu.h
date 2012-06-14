#ifndef MENU_H
#define MENU_H

#include "dec.h"
#include "functions.h"

class Menu { //trieda od ktorej je dedene PageInMenu a PopMenu
		SDL_Surface *cursor; //kurzor
		SDL_Color col_active; //farba vybranej polozky
		SDL_Color col_passive; //farba polozky	
	protected:
		int activeLine; //aktivny riadok
	public:
		Menu();
		~Menu();

		vector<string> lines; //riadky v menu
		vector<string>::iterator it_lines;

		void setCursor(const char *cur_path); //zmena kurzoru
		void setActiveColor(SDL_Color color); //zmena aktivnej farby
		void setPassiveColor(SDL_Color color); //zmena pasivnej(nevybratej) farby
		SDL_Color getActiveColor(); //vratenie aktivnej farby
		SDL_Color getPassiveColor(); //vratenie pasivnej farby
		void addLine(string newLine); //pridanie riadku na koniec menu	
		void changeLine(int n); //riadok cislo n sa stane aktivnym
		void deleteLines(); //zmazanie vsetkych riadkov
		int getActiveLine(); //vratenie activeLine
		string getActiveLineAsString(); //vratenie activeLine stringu
		void setActiveLine(string new_line);
};

class PageInMenu: public Menu{ //jedna strana v menu
		SDL_Surface *bg; //pozadie
		TTF_Font *font; //font v menu
	public:
		PageInMenu();
		PageInMenu(SDL_Surface& new_bg, TTF_Font& new_font);
		~PageInMenu();

		void setBackground(SDL_Surface &new_bg); //zmena pozadia
		SDL_Surface& getBackground(); //vratenie pozadia
		void setFont(TTF_Font &new_font); //zmena fontu
		TTF_Font& getFont(); //vratenie fontu	
		void changeLine(unsigned int x, unsigned int y); //riadok pod kurzorom sa stane aktivny	
};

class CompleteMenu{
		vector<PageInMenu> pages; //strany	
		unsigned int no_actualPage; //cislo aktualnej strany
	public:
		CompleteMenu();
		~CompleteMenu();

		vector<PageInMenu>::iterator it_actualPage; //iterator aktualnej strany

		unsigned int getActualPage(); //vratenie no_actualPage
		void addPage(PageInMenu new_page); //pridanie novej strany do menu
		void changeActualPage(unsigned int n); //zmena aktualnej strany
		void draw(SDL_Surface *src); //vykreslenie aktualnej strany

};

bool menuEventChangePage(CompleteMenu &m); //co vykonat ak sa stlaci enter, alebo klikne mys
void menuEventKeyUp(CompleteMenu &m); //
void menuEventKeyDown(CompleteMenu &m);
bool menuEventKeyESC(CompleteMenu &m);
bool processEventsInMenu(CompleteMenu &m);

#endif