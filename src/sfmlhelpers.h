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

	// three sprites - one for the prefix selection, word search selection, go button for search
	sf::Sprite prefixbutton;
	sf::Sprite wordsearchbutton;
	sf::Sprite gobutton;

	TextureManager();
};

class DisplayWindow{
	private:
		sf::RenderWindow window;
		sf::Font font;
		
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

		// sets up the text that appears after the results are calculated
		void configureResultsText();

		// updates the user input text as they are typing
		void updateInputText(sf::Event &event);

		// updates the results under the trie and hash map implementations, along with the time taken
		void updateResults();

		// runs the program with all components
		void run();

};