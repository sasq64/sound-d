#pragma once
#include <memory>
#include <mutex>

template <typename T> class SafePointer {
public:
	struct LockHolder {
		LockHolder(T *ptr, std::unique_lock<std::mutex> &&g) : ptr(ptr), guard(std::move(g)) {}

		LockHolder(LockHolder &&l) : guard(std::move(l.guard)), ptr(std::move(l.ptr)) {}

		T *operator->() const { return ptr; }

		T *get() const { return ptr; }

		explicit operator bool() { return ptr != nullptr; }

		void unlock() { guard.unlock(); }

		std::unique_lock<std::mutex> guard;
		T *ptr;
	};

	LockHolder operator->() const {
		std::unique_lock<std::mutex> guard(*m);
		return LockHolder(ptr.get(), std::move(guard));
	}

	LockHolder aquire() const {
		std::unique_lock<std::mutex> guard(*m);
		return LockHolder(ptr.get(), std::move(guard));
	}

	SafePointer() : m(std::make_shared<std::mutex>()) {}
	SafePointer(std::shared_ptr<T> ptr) : ptr(ptr), m(std::make_shared<std::mutex>()) {}
	SafePointer(T *ptr) : ptr(std::shared_ptr<T>(ptr)), m(std::make_shared<std::mutex>()) {}

	template <typename P> SafePointer<T> operator=(P t) {
		std::unique_lock<std::mutex> guard(*m);
		ptr = t;
		return *this;
	}

	SafePointer<T> operator=(SafePointer<T> p) {
		auto m2 = m;
		std::unique_lock<std::mutex> guard(*m);
		// TODO: Lock other mutex?
		ptr = p.ptr;
		m = p.m;
		return *this;
	}

	SafePointer<T> operator=(std::shared_ptr<T> p) {
		std::unique_lock<std::mutex> guard(*m);
		ptr = p;
		return *this;
	}

	bool operator==(T *t) const {
		std::unique_lock<std::mutex> guard(*m);
		return ptr.get() == t;
	}

	bool operator!=(T *t) const {
		std::unique_lock<std::mutex> guard(*m);
		return ptr.get() != t;
	}

	explicit operator bool() const {
		std::unique_lock<std::mutex> guard(*m);
		return ptr.get() != nullptr;
	}

private:
	std::shared_ptr<T> ptr;
	std::shared_ptr<std::mutex> m;
};

template <typename T> SafePointer<T> make_safepointer(std::shared_ptr<T> ptr) {
	return SafePointer<T>(ptr);
}

