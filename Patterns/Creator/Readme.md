# Content
I want to implement something like 'named constructors', in particular:
Texture::createFromFile(std::string filename);
Texture::createFromSomethingElse(std::string something);

The creator must must be usable as follows (or similar):
	* Something like: Texture myTex::loadFromFile(std::string filename);
	* Something like: Texture* myOtherTex = myTex::loadFromSomethingElse(std::string something);