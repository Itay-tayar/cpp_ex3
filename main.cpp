#include <iostream>
#include <vector>
#include <algorithm>

auto _getValue(int key, const std::vector<std::pair<int, int> >& vec )
{
    return find_if( vec.begin(), vec.end(),
                    [&key](const std::pair<int, int>  &element) -> bool
                    { return element.first == key;} );
}

int main()
{

    std::vector<std::pair<int, int> > vec = {std::make_pair(1, 4),
                                              std::make_pair(2, 0),
                                              std::make_pair(3, 0)};
    auto it  = _getValue(2,vec);

    std::cout <<distance(vec.begin(), it);
}

