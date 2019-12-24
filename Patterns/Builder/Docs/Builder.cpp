Quad quad;
quad.setPosition(1, 1);
quad.setScale(1, 1);

// Quad quad;
// quad.setPosition(1, 1).setScale(1, 1);
set(quad).with<setPosition>(1, 1).with<setScale>(1, 1);

template <class T>
class Builder {
	T& _target;
public:
	Builder(T& target) : _target(target) { }
	template <class f, A1, A2>
	Builder with(const A1& a1, const A2& a2) {
		_target.f(a1, a2);
		return _target;
	}
}

typedef set Builder;

template <class T>
Builder& set(T& object) {
	Builder builder;
}