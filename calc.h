// You need to submit this file
#pragma once

#include <any>
#include "visitor.h"

struct calculator : visitor {
    static bool is_double(const std::any &v) { return std::any_cast<const double>(&v) != nullptr; }

    static double to_double(const std::any &v) {
        if (auto pd = std::any_cast<const double>(&v)) return *pd;
        return static_cast<double>(std::any_cast<const long long>(v));
    }

    static long long to_ll(const std::any &v) { return std::any_cast<const long long>(v); }

    std::any visit_num(num_node *n) override { return n->number; }

    std::any visit_add(add_node *n) override {
        auto lv = visit(n->lnode);
        auto rv = visit(n->rnode);
        if (is_double(lv) || is_double(rv)) {
            return to_double(lv) + to_double(rv);
        }
        return to_ll(lv) + to_ll(rv);
    }

    std::any visit_sub(sub_node *n) override {
        auto lv = visit(n->lnode);
        auto rv = visit(n->rnode);
        if (is_double(lv) || is_double(rv)) {
            return to_double(lv) - to_double(rv);
        }
        return to_ll(lv) - to_ll(rv);
    }

    std::any visit_mul(mul_node *n) override {
        auto lv = visit(n->lnode);
        auto rv = visit(n->rnode);
        if (is_double(lv) || is_double(rv)) {
            return to_double(lv) * to_double(rv);
        }
        return to_ll(lv) * to_ll(rv);
    }

    std::any visit_div(div_node *n) override {
        auto lv = visit(n->lnode);
        auto rv = visit(n->rnode);
        if (is_double(lv) || is_double(rv)) {
            return to_double(lv) / to_double(rv);
        }
        return to_ll(lv) / to_ll(rv);
    }

    ~calculator() override = default;
};
