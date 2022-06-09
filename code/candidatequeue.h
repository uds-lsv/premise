#ifndef CANDIDATEQUEUE_H
#define CANDIDATEQUEUE_H

template<typename T, typename Comparator>
class OnlyBestQueue
{
    public:
        void push(const T& x) {
            if(empty)
            {
                empty = false;
                best = x;
            }
            else if(comp(best, x))
            {
                best = x;
            }
        }

        bool isEmpty()
        {
            return empty;
        }

        T top()
        {
            assert(!empty);
            return best;
        }

    private:
        bool empty = true;
        T best;
        Comparator comp;

};

#endif