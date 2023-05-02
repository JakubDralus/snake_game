#include<iostream>
#include "Apple.h"
#include "util.h"

Apple::Apple(int x, int y) {
	m_coords.x = std::move(x);
	m_coords.y = std::move(y);
	display();
	std::cout << "Apple set in: " << m_coords.x/11 << " " << m_coords.y/11 << std::endl;
}

void Apple::display() {
	setPixel5x5(m_coords.x, m_coords.y, m_apple_color, background_color);
}