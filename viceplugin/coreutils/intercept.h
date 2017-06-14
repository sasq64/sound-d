#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>

template <typename T> std::unordered_map<std::string, std::vector<std::function<bool(T&)>>> interceptions() {
		static std::unordered_map<std::string, std::vector<std::function<void(T&)>>> im;
		return im;
}

template <typename T> bool intercept_point(const std::string &id, T& val) {
	auto it = interceptions<T>().find(id);
	if(it != interceptions<T>().end()) {
		it->second(val);
		return true;
	}
	return false;
}


template <typename T> void intercept(const std::string &id, std::function<void(T&)> cb) {
	interceptions<T>().insert(make_pair(id, cb));
}
