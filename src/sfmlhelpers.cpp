#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include "sfmlhelpers.h"

// a lot of this has been inspired by and copied from my minesweeper project - Rylee

TextureManager::TextureManager() : prefixbutton(prefixsearch), wordsearchbutton(wordsearch), gobutton(searchbar){
	prefixsearch.loadFromFile("assets/images/prefixsearch.png");
	prefixsearch_pressed.loadFromFile("assets/images/prefixsearch_pressed.png");
	wordsearch.loadFromFile("assets/images/wordsearch.png");
	wordsearch_pressed.loadFromFile("assets/images/wordsearch_pressed.png");
	searchbar.loadFromFile("assets/images/searchbar.png");
}

// taken from the minesweeper specs
void setText(sf::Text &text, float x, float y){
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin({textRect.position.x + textRect.size.x/2.0f,
	textRect.position.y + textRect.size.y/2.0f});
	text.setPosition(sf::Vector2f(x, y));
}

void DisplayWindow::setupText(sf::Text &text, std::string message, int size, sf::Color color, std::pair<int, int> pos, bool bold){
	text.setString(message);
	text.setCharacterSize(size);
	text.setFillColor(color);
	setText(text, pos.first, pos.second);
	if (bold)
		text.setStyle(sf::Text::Bold);
}


DisplayWindow::DisplayWindow() :
	window(sf::VideoMode({1920,1080}), "TransGator", sf::Style::Close),
	title(font),
	prefixdescription(font),
	wholedescription(font),
	inputword(font),
  	trietitle(font),
  	trietime(font),
  	trieresults(font),
  	hashtitle(font),
  	hashtime(font),
  	hashresults(font){

	// load our text font
	font.openFromFile("assets/arialuni.ttf");

	// set all bools to false
	prefix_on = false;
	whole_on = false;
	perform_search = false;

	configureStaticText();
}

/*
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
*/

void DisplayWindow::configureStaticText(){
	setupText(title, "TRANSGATOR: ENGLISH TO SPANISH", 60, sf::Color::Black, {960, 80}, true);
}

// signifcant help from my minesweeper project with the name typing and checking - Rylee
void DisplayWindow::updateInputText(sf::Event &event){
	// check if text is being entered
	if (auto textEvent = event.getIf<sf::Event::TextEntered>()){

		char letter = static_cast<char>(textEvent->unicode);

		// checking the character is a valid letter
		if ((letter >= 'A' && letter <= 'Z') || (letter >= 'a' && letter <= 'z')){
			if (input.size() < 26){ // arbitrary number for now until we figure out the max char
				input += letter;
			}
		}
	}

	// check if they're deleting characters or if they press enter 
	else if (auto keyEvent = event.getIf<sf::Event::KeyPressed>()){

		// check for backspace and remove letter
		if (keyEvent->code == sf::Keyboard::Key::Backspace && input.size() > 0){
			input.pop_back();
		}
		
		// check for enter and perform the search
		if (keyEvent->code == sf::Keyboard::Key::Enter && input.size() > 0){
			perform_search = true;
		}
	}
}

void DisplayWindow::updateResults(){

}

void DisplayWindow::run(){

	while (window.isOpen())
	{
		window.clear(sf::Color::White);
		
		while (auto event = window.pollEvent()){

			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
	
		}
		window.draw(title);
		window.display();
	}
}