echo "#pragma once" > all.hpp
find .. -name "*.hpp" | sed 's/\(^.*$\)/#include "\1"/' >> all.hpp
