#include <iostream>
#include <string>
#include <deque>
#include <vector>
using namespace std;

class MSD
{
private:
    const int M[3][3] = {
        {0, 1, 1},
        {0, 0, 0},
        {-1, 0, 1}};
    const int T[3][3] = {
        {-1, -1, 0},
        {-1, 0, 1},
        {0, 1, 1}};
    const int W[3][3] = {
        {0, 1, 0},
        {1, 0, -1},
        {0, -1, 0}};
    const int T_[3][3] = {
        {-1, 0, 0},
        {0, 0, 0},
        {0, 0, 1}};
    const int W_[3][3] = {
        {0, -1, 0},
        {-1, 0, 1},
        {0, 1, 0}};
    int R;
    int G;
    deque<int> aux_R;
    deque<int> aux_G;
    deque<int> transform_bit(int R, deque<int> aux)
    {
        if (R < 0)
        {
            throw "R must be positive";
        }
        while (R)
        {
            aux.push_front(R % 2);
            R = R / 2;
        }
        return aux;
    };
    deque<int> transform_msd(int R, deque<int> aux)
    {
        bool is_negative_number = false;
        if (R < 0)
        {
            is_negative_number = true;
            R = -R;
        }
        deque<int> result = transform_bit(R, aux);
        if (is_negative_number)
            for (int &num : result)
                num = -num;
        return result;
    };
    void resize(deque<int> &aux_R, deque<int> &aux_G)
    {
        int size_R = aux_R.size();
        int size_G = aux_G.size();
        if (size_R > size_G)
            resize(aux_G, aux_R);
        int length = size_G - size_R;
        while (length > 0)
        {
            aux_R.push_front(0);
            length--;
        }
    };
    deque<int> transform_t(deque<int> aux_R, deque<int> aux_G)
    {
        deque<int> result;
        if (aux_R.size() != aux_G.size())
            resize(aux_R, aux_G);
        for (int i = 0; i < aux_R.size(); i++)
            result.push_back(T[aux_G[i] + 1][aux_R[i] + 1]);
        return result;
    };
    deque<int> transform_w(deque<int> aux_R, deque<int> aux_G)
    {
        deque<int> result;
        if (aux_R.size() != aux_G.size())
            resize(aux_R, aux_G);
        for (int i = 0; i < aux_R.size(); i++)
            result.push_back(W[aux_G[i] + 1][aux_R[i] + 1]);
        return result;
    };
    deque<int> transform_t_(deque<int> aux_R, deque<int> aux_G)
    {
        deque<int> result;
        if (aux_R.size() != aux_G.size())
            resize(aux_R, aux_G);
        for (int i = 0; i < aux_R.size(); i++)
            result.push_back(T_[aux_G[i] + 1][aux_R[i] + 1]);
        return result;
    };
    deque<int> transform_w_(deque<int> aux_R, deque<int> aux_G)
    {
        deque<int> result;
        if (aux_R.size() != aux_G.size())
            resize(aux_R, aux_G);
        for (int i = 0; i < aux_R.size(); i++)
            result.push_back(W_[aux_G[i] + 1][aux_R[i] + 1]);
        return result;
    };
    deque<int> transform_m(deque<int> aux_R, int x)
    {
        deque<int> result;
        for (int i = 0; i < aux_R.size(); i++)
            result.push_back(M[x + 1][aux_R[i] + 1]);
        return result;
    };
    vector<deque<int>> bit_mul_transform(deque<int> aux_R, deque<int> aux_G)
    {
        vector<deque<int>> result;
        int g_size = aux_G.size();
        for (auto g : aux_G)
        {
            deque<int> mid_result = transform_m(aux_R, g);
            for (int i = --g_size; i > 0; i--)
                mid_result.push_back(0);
            for (int element : mid_result)
            {
                cout << element << " ";
            }
            cout << endl;
            result.push_back(mid_result);
        }
        return result;
    };
    deque<int> msd_add_by_mul(deque<int> aux_R, deque<int> aux_G)
    {
        deque<int> u = transform_t(aux_R, aux_G);
        deque<int> w = transform_w(aux_R, aux_G);
        u.push_back(0); // u back 0
        deque<int> u_ = transform_t_(u, w);
        deque<int> w_ = transform_w_(u, w);
        u_.push_back(0); // u_ back 0
        deque<int> result = transform_t(u_, w_);
        return result;
    };
    int abs(int x)
    {
        if (x < 0)
            x = -x;
        return x;
    };

public:
    MSD(){};
    deque<int> msd_add(int R, int G)
    {
        this->R = R;
        this->G = G;
        aux_R.clear();
        aux_G.clear();
        if (!aux_R.empty() || !aux_G.empty())
            throw "deque is not empty";
        aux_R = transform_msd(R, aux_R);
        aux_G = transform_msd(G, aux_G);
        deque<int> u = transform_t(aux_R, aux_G);
        deque<int> w = transform_w(aux_R, aux_G);
        u.push_back(0); // u back 0
        deque<int> u_ = transform_t_(u, w);
        deque<int> w_ = transform_w_(u, w);
        u_.push_back(0); // u_ back 0
        deque<int> result = transform_t(u_, w_);
        while (!result.front())
            result.pop_front();
        return result;
    };
    deque<int> msd_mul(int R, int G)
    {
        if (abs(R) > abs(G))
            return msd_mul(G, R);
        this->R = R;
        this->G = G;
        aux_R.clear();
        aux_G.clear();
        if (!aux_R.empty() || !aux_G.empty())
            throw "deque is not empty";
        aux_R = transform_msd(R, aux_R);
        aux_G = transform_msd(G, aux_G);
        vector<deque<int>> s = bit_mul_transform(aux_R, aux_G);
        if (s.size() < 2)
            throw "detail error";
        int s_size = s.size();
        deque<int> result = msd_add_by_mul(s[s_size - 1], s[s_size - 2]);
        int i = s.size() - 3;
        while (i >= 0)
        {
            result = msd_add_by_mul(result, s[i]);
            i--;
        }
        while (!result.front())
            result.pop_front();
        return result;
    };
};

int main()
{

    MSD msd = MSD();
    deque<int> result = msd.msd_mul(-10, 2);
    for (int num : result)
        cout << num << " ";
    cout << endl;
    system("pause");
    return 0;
}
