#include <iostream>
#include <map>
#include <vector>
#include <tuple>

struct Material
{
	Material(std::string texture_, char color_)
		: texture(texture_), color(color_)
	{ }
	
	std::string texture;
	
	char color;	
};

bool operator < (const Material &a, const Material& b)
{
	return std::tie(a.texture, a.color) < std::tie(b.texture, b.color);
}	

struct Vertex
{
	Vertex(float x_, float y_) 
		: x(x_), y(y_)
	{ }
	
	float x;
	float y;
};

struct example
{
    int x; int y;
};

bool operator < (const example& a, const example& b) 
{
	return std::make_pair(a.x,a.y) < std::make_pair(b.x,b.y) ; 
}

int main()
{
	typedef std::map<Material*, std::vector<Vertex> > Grouping; 

	
    // example variable = { 1, 2 };
    // std::map< example, int> something;
    // something[variable] = 3;
	
	Material mat1("tex1", 1);
	Material mat2("tex2", 2);
	
	Grouping grouping;	
	grouping[&mat1].push_back(Vertex(0, 1));
	grouping[&mat2].push_back(Vertex(2, 3));
	grouping[&mat2].push_back(Vertex(4, 5));
	grouping[&mat1].push_back(Vertex(6, 7));
	
	for ( Grouping::iterator it = grouping.begin(); it != grouping.end(); it++ ) {
		std::cout << "Material: " << it->first->texture << " " << (int)it->first->color << std::endl;  
		
		for (std::size_t i = 0; i < it->second.size(); i++)
			std::cout << "\t" << it->second[i].x << it->second[i].y << std::endl;
	}
}