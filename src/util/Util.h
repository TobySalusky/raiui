//
// Created by Tobiathan on 1/17/23.
//

#pragma once

#include <numeric>
#include <functional>
#include <optional>
#include <variant>
#include <memory>
#include <ranges>


namespace tutil {
    template<typename Container, typename UnaryOp>
    auto transform_sum(Container container, UnaryOp transformFunc) {
        return std::transform_reduce(container.begin(), container.end(), decltype(transformFunc(*(container.begin()))){ 0 }, std::plus<>(), transformFunc);
    }

    template<typename Container, typename UnaryOp>
    auto transform_max(Container container, UnaryOp transformFunc) {
        return std::transform_reduce(container.begin(), container.end(), decltype(transformFunc(*(container.begin()))){ 0 }, [](auto l, auto r) { return std::max(l, r); }, transformFunc);
    }

    template<typename Target, typename ...Ts>
    bool exists_with(std::optional<std::variant<Ts...>> optionalVariant){
        return optionalVariant && std::holds_alternative<Target>(*optionalVariant);
    }

    template<typename Target, typename ...Ts>
    std::optional<Target> pick(std::optional<std::variant<Ts...>> optionalVariant){
        return exists_with<Target>(optionalVariant) ? std::optional<Target>{ std::get<Target>(*optionalVariant) } : std::nullopt;
    }

    template<typename Target, typename ...Ts>
    std::optional<Target> pick(std::variant<Ts...> variant){
        return std::holds_alternative<Target>(variant) ? std::optional<Target>{ std::get<Target>(variant) } : std::nullopt;
    }

//    float zero_clamp(float f) {
//        return std::max(0.0f, f);
//    }


    // >> vectorization of ranges ===============================
    struct to_vector {};

    template <std::ranges::range Range>
    auto operator|(Range&& r, to_vector helper) {
        std::vector<std::ranges::range_value_t<Range>> v;

        if constexpr(std::ranges::sized_range<Range>) {
            v.reserve(std::ranges::size(r));
        }

        std::ranges::copy(r, std::back_inserter(v));

        return v;
    }

    // >> string splitting to string views ===============================
    struct str_split{
        explicit str_split (char splitOn) : splitOn(splitOn) {}
        char splitOn;
    };

    template <std::ranges::range Range>
    auto operator|(Range&& r, str_split splitter) {
        return r | std::views::split(splitter.splitOn) | std::views::transform([](auto&& r){ return std::string_view(&*r.begin(), std::ranges::distance(r)); });
    }

    struct str_split_v{
        explicit str_split_v (char splitOn) : splitOn(splitOn) {}
        char splitOn;
    };

    template <std::ranges::range Range>
    auto operator|(Range&& r, str_split_v splitter) {
        return r | std::views::split(splitter.splitOn) | std::views::transform([](auto&& r){ return std::string_view(&*r.begin(), std::ranges::distance(r)); }) | to_vector();
    }


    struct join_str {};
    template <std::ranges::range Range>
    auto operator|(Range&& r, join_str joiner) {
        std::string s;

        for (auto&& val : r) {
            s += std::string(val);
        }

        return s;
    }

    struct join_on {
        explicit join_on (char joinOn) : joinOn(joinOn) {}
        char joinOn;
    };
    template <std::ranges::range Range>
    auto operator|(Range&& r, join_on joiner) {
        std::string s;

        bool first = true;
        for (auto& val : r) {
            if (!first) { s += joiner.joinOn; }
            s += std::string(val);
            first = false;
        }

        return s;
    }

    struct drop_last {};
    template <std::ranges::range Range>
    auto operator|(Range&& r, drop_last helper) {
        return r | to_vector() | std::views::reverse | std::views::drop(1) | std::views::reverse;
    }

    struct drop_last_v {};
    template <std::ranges::range Range>
    auto operator|(Range&& r, drop_last_v helper) {
        return r | std::views::reverse | std::views::drop(1) | std::views::reverse;
    }

    struct unwrap_or_filter {};
    template <std::ranges::range Range>
    auto operator|(Range&& r, unwrap_or_filter helper) {
        return r | std::views::filter([](auto o) { return o.has_value(); }) | std::views::transform([](auto o) { return *o; });
    }

    template <typename T, typename R>
    struct reduce {
        explicit reduce (T init, std::function<void(T&, R)> func) : init(init), func(func) {}
        T init;
        std::function<void(T&, R)> func;
    };
    template <std::ranges::range Range, typename T>
    auto operator|(Range&& r, reduce<T, std::ranges::range_value_t<Range>> reducer) {

        std::for_each(r.begin(), r.end(), [&](auto&& it) {return reducer.func(reducer.init, it); });
        return reducer.init;
    }

    template<typename T>
    struct ref_collection_iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::iter_difference_t<size_t>;
        using value_type        = T;
        using pointer           = T*;  // or also value_type*
        using reference         = T&;  // or also value_type&

        ref_collection_iterator(std::vector<pointer>* v, size_t i) : v(v), i(i) {}

        reference operator*() const { return *((*v)[i]); }
        pointer operator->() { return (*v)[i]; }

        // Prefix increment
        ref_collection_iterator& operator++() { i++; return *this; }

        // Postfix increment
        ref_collection_iterator operator++(int) { ref_collection_iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const ref_collection_iterator& a, const ref_collection_iterator& b) { return a.i == b.i; };
        friend bool operator!= (const ref_collection_iterator& a, const ref_collection_iterator& b) { return a.i != b.i; };

    private:
        std::vector<pointer>* v;
        size_t i;
    };

    struct collect_refs {};
    template<typename T>
    struct ref_collection {
        ref_collection(const std::vector<T*>& data) {
            _data = data;
            _dataPtr = &_data;
        }

        ref_collection_iterator<T> begin() const { return { _dataPtr, 0 }; }
        ref_collection_iterator<T> end() const { return { _dataPtr, size() }; }

        [[nodiscard]] size_t size() const { return _data.size(); }
        [[nodiscard]] bool empty() const { return _data.empty(); }

    private:
        std::vector<T*> _data;
        std::vector<T*>* _dataPtr;
    };

    template <std::ranges::range Range>
    auto operator|(Range&& r, collect_refs collector) {
        std::vector<std::ranges::range_value_t<Range>*> vec = {};
        for (std::ranges::range_value_t<Range>& element : r) {
            vec.push_back(&element);
        }
        return ref_collection<std::ranges::range_value_t<Range>>(vec);
    }


    // https://articles.emptycrate.com/2016/09/14/destructuring_containers.html
    template<typename T, size_t ... S>
    auto destructure_impl(T &t, std::index_sequence<S...>)
    {
        return std::forward_as_tuple(*std::next(std::begin(t), S)...);
    }

    template<size_t Count, typename T>
    auto destructure(T &t)
    {
        if (t.size() < Count) {
            throw std::runtime_error("Tried to destructure a container into too many elements!");
        }
        return destructure_impl(t, std::make_index_sequence<Count>());
    }

    template<typename T, size_t ... S>
    auto dole_impl(T &t, std::index_sequence<S...>)
    {
        return std::make_tuple(*std::next(std::begin(t), S)...);
    }

    template<size_t Count, typename T>
    auto dole(T &t)
    {
        if (t.size() < Count) {
            throw std::runtime_error("Tried to destructure a container into too many elements!");
        }
        return dole_impl(t, std::make_index_sequence<Count>());
    }

    template<size_t Count, typename T>
    auto dole(T&& t)
    {
        if (t.size() < Count) {
            throw std::runtime_error("Tried to destructure a container into too many elements!");
        }
        return dole_impl(t, std::make_index_sequence<Count>());
    }
}
