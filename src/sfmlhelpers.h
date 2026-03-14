#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "dictionary.h"
#include "HashMap.h"
#include "Trie.h"

struct TextureManager{
	// button textures
	sf::Texture prefixsearch;
	sf::Texture prefixsearch_pressed;
	sf::Texture wordsearch;
	sf::Texture wordsearch_pressed;
	sf::Texture searchbar;
	sf::Texture gotexture;

	// set up image sprites
	sf::Sprite prefixbutton;
	sf::Sprite wordsearchbutton;
	sf::Sprite gobutton;
	sf::Sprite searchbarsprite;

	TextureManager();
};

class DisplayWindow{
	private:
		TextureManager textures;

		sf::RenderWindow window;
		sf::Font font;
		sf::RectangleShape horizontaldivider;
		sf::RectangleShape verticaldivider;
		sf::RectangleShape cursor;
		bool cursorvisible = true;
		sf::Clock cursorclock;


		
		sf::Text title;
		sf::Text prefixdescription;
		sf::Text wholedescription;
		sf::Text inputword;
		sf::Text trietitle;
		sf::Text trietime;
		sf::Text trieresults;
		sf::Text hashtitle;
		sf::Text hashtime;
		sf::Text hashresults;

		std::string input;
		bool prefix_on;
		bool whole_on;
		bool perform_search;

	public:

		// constructor
		DisplayWindow();

		// sets up the text with all properties needed
		void setupText(sf::Text &text, std::string message, int size, sf::Color color, std::pair<int, int> pos, bool bold);

		// sets up text that doesn't change on the window
		void configureStaticText();

		// updates the user input text as they are typing
		void updateInputText(sf::Event &event);

		// implements a blinking cursor while the user is typing
		void updateCursor();

		// given a click event, determine if they clicked any buttons and deal with the buttons
		void buttonClick(sf::Event &event);

		// if an option is picked and a word is entered and they press go, run the translator
		void runSearch();

		// updates the results under the hash map implementations, along with the time taken
		void updateTrieResults();

		// updates the results under the trie implementation, along with the time taken
		void updateHashResults();

		// display all the text on screen
		void drawText();

		// draw and display the buttons
		void drawButtons();

		// runs the program with all components
		void run();

};