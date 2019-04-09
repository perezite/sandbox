#include <iostream>
#include <string>

class Texture {
	std::size_t _id;
public:
	Texture() { 
		static std::size_t counter = 0;
		_id = counter++;
		std::cout << "Texture::ctor()" << std::endl;
	}

	virtual ~Texture() {
		std::cout << "Texture::dtor() " << _id << std::endl;
	}

	void loadFromFile(std::string filename) {
		std::cout << "Load texture from file: " << filename << std::endl;
	}

	static Texture* createFromFile(const std::string& filename) {
		Texture* texture = new Texture();
		texture->loadFromFile(filename);
		return texture;
	}
};

void main() {
	{
		std::cout << "Test1" << std::endl;
		Texture texture1;
		texture1.loadFromFile("bunny.png");

		std::cout << "Test2" << std::endl;
		Texture* texture2 = Texture::createFromFile("bunny.png");
		delete texture2;

		std::cout << "Test3" << std::endl;
		Texture tex;

		// Object<Texture> tex(new Texture())
		// Object<Texture> tex()
		// Object<Texture> texture
		// Texture* texture = create<Texture>.loadFromFile("bunny.png");
		// Texture::createFromFile(Texture tex);
	}

	{
		std::cout << "Test4" << std::endl;
		Texture tex;
	}

	{
		// Object<Texture> blub;
		// Texture tex(Texture::loadFromFile("bunny.png"));
		// Texture tex::create(...);
		// Object<Texture> texture(new Texture());
	}

	std::cout << "Press any key to continue" << std::endl;
	std::cin.get();
}