//
// Created by Tuncel  Mustafa Anil on 7/17/18.
//

#ifndef SC_DNA_INFERENCE_H
#define SC_DNA_INFERENCE_H

#include "Tree.h"
#include <vector>
#include <random>

class Inference {
/*
 * Contains functionality to perform monte carlo markov chains (mcmc) inference
 * */
private:
    Tree *t;
    Tree *t_prime;
    std::vector<std::vector<double>> t_scores;
    std::vector<double> t_sums;

    std::vector<unordered_map<int, double>> t_prime_scores;
    std::vector<double> t_prime_sums;

public:
    Inference(u_int ploidy=2);

    ~Inference();
    void destroy();
    void compute_t_table(const vector<vector<int>> &D, const vector<int>& r);
    Node * prune_reattach(const vector<vector<int>> &D, const vector<int> &r);
    bool comparison(int m);

    void infer_mcmc(const vector<vector<int>> &D, const vector<int>& r);

    void w_prune_reattach();

    void swap();
    void w_swap();


    void random_initialize(); // randomly initializes a tree and copies it into the other
    void test_initialize(); // initializes the trees based on the test example
};



void Inference::random_initialize() {

}

void Inference::test_initialize() {

    // build tree
    t->random_insert({{0, 1}, {1, 1}});
    t->insert_at(1,{{1, 1}, {2, 1}});
    t->insert_at(2,{{0, -1}});
    t->insert_at(2,{{3, -1}});
    t->insert_at(1,{{1, 1}});


}

Inference::Inference(u_int ploidy) {
    t = new Tree(ploidy);
    t_prime =new Tree(ploidy);

}

Inference::~Inference() {
    destroy();
}

Node * Inference::prune_reattach(const vector<vector<int>> &D, const vector<int> &r) {
    /*
     * Prunes and reattaches to t_prime
     * */
    using namespace std;
    Node* attached_node = t_prime->prune_reattach();

    if (attached_node != nullptr)
    {
        int j = 0;
        for (auto const &d: D)
        {
            int sum_d = accumulate( d.begin(), d.end(), 0.0);
            attached_node->parent->log_score = t_scores[j][attached_node->parent->id];
            t_prime->compute_stack(attached_node, d, sum_d,r);
            t_prime_scores.push_back(t_prime->get_children_id_score(attached_node));
            j++;
        }
        int i = 0;
        for (auto const &d: D)
        {
            vector<double> old_vals;
            vector<double> new_vals;

            for (auto &u_map : t_prime_scores[i])
            {
                old_vals.push_back(t_scores[i][u_map.first]);
                new_vals.push_back(u_map.second);
            }

            auto res =MathOp::log_replace_sum(t_sums[i],old_vals,new_vals);
            t_prime_sums.push_back(res);
            i++;
        }
        return attached_node;
    }
    else
        return nullptr;
}

void Inference::compute_t_table(const vector<vector<int>> &D, const vector<int>& r) {

    int n = static_cast<int>(D.size());
    for (int i = 0; i < n; ++i)
    {
        this->t->compute_tree(D[i], r);
        auto scores_vec = this->t->get_scores();
        this->t_scores.push_back(scores_vec);
        this->t_sums.push_back(MathOp::log_sum(scores_vec));
    }
    // update t_prime
    // calls the copy constructor
    *t_prime = *t;

}

void Inference::destroy() {
    delete t;
    t = nullptr;
    delete t_prime;
    t_prime = nullptr;
}

bool Inference::comparison(int m) {
    // m is

    double log_post_t = 0.0;
    double log_post_t_prime = 0.0;

    double t_sum = accumulate( t_sums.begin(), t_sums.end(), 0.0);
    int t_n = t->get_n_nodes();
    log_post_t = t_sum - (t_n -1 + m ) * log(t_n+1);

    double t_prime_sum = accumulate( t_prime_sums.begin(), t_prime_sums.end(), 0.0);
    int tp_n = t_prime->get_n_nodes();
    log_post_t_prime = t_prime_sum - (tp_n -1 + m ) * log(tp_n+1);

    double acceptance_prob = exp(log_post_t_prime - log_post_t);

    if (acceptance_prob > 1)
        return true;
    else
    {
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution(0.0,1.0);
        double rand_val = distribution(generator);

        if (acceptance_prob > rand_val)
            return true;
        else
            return false;
    }
}

void Inference::infer_mcmc(const vector<vector<int>> &D, const vector<int>& r) {

    int m = static_cast<int>(D.size());
    int n_accepted = 0;
    int n_rejected = 0;
    for (int i = 0; i < 5000; ++i) {

        // apply the move to t_prime
        Node* node = prune_reattach(D,r);

        if (node == nullptr)
            continue;
        else
        {
            // compare
            bool accepted = comparison(m);

            // update trees and the matrices
            if (accepted)
            {
                n_accepted++;
                /*
                 * TODO
                 * 1. copy t_prime_sums to t_sums
                 * 2. replace only the existing cols in t_scores with t_prime_scores
                 * 3. perform a t=t_prime
                 *
                 * */

            }
            else
            {
                n_rejected++;
                /*
                 * TODO
                 * 1. delete t_prime_sums
                 * 2. delete t_prime_scores
                 * 3. perform a t_prime=t
                 *
                 * */
            }


        }

    }
    cout<<"n_accepted: "<<n_accepted<<endl;
    cout<<"n_rejected: "<<n_rejected<<endl;


}


#endif //SC_DNA_INFERENCE_H
