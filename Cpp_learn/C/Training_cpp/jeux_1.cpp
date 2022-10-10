#include <memory>
#include "num.hpp" // We use it for the variable name...
#include <vector>

using infos_soldier = num;
using soldier_w = std::weak_ptr<infos_soldier>;
using soldier = std::shared_ptr<infos_soldier>;
using  army = std::vector<soldier>;
using group_army = std::vector<soldier_w>;

using infos_wepon =  num;
using weapon = std::weak_ptr<infos_wepon>;
using arsenal  = std::vector<weapon>;


void delete_somone(army& p, const std::string& target);
void print_army(army& p);
void print_weapo(arsenal& a);



int main(int argc, char* argv[]) {

    army A;
    arsenal fusil_dassault,sniper,fusil_pompe,pistolet;
    {
        auto soldier_1 = std::make_shared<infos_soldier>("Gérar");
        auto soldier_2 = std::make_shared<infos_soldier>("Kevin");
        auto soldier_3 = std::make_shared<infos_soldier>("Hervé");

        auto weapon_1 = std::make_shared<infos_wepon>("Magnum");
        auto weapon_2 = std::make_shared<infos_wepon>("Desertigle");
        auto weapon_3 = std::make_shared<infos_wepon>("Rocket_kekette");

        A={soldier_1,soldier_2,soldier_3};
        pistolet = {weapon_1,weapon_2,weapon_3};

        // Deleate a soldier
        delete_somone(A,"Hervé");
        print_army(A);
        
    }
}
void delete_somone(army& p, const std::string& target){
    for(auto i=p.begin();i<p.end();++i){
    soldier sol= *i;
    if(std::string(*sol)==target){
        p.erase(i);
        return;
    }
    }
}

void print_army(army& p){
    std::cout << scope_indent;
    for(auto i =p.begin();i<p.end();){
        if(*i!=nullptr){
        std::cout << '\"' << std::string(**i) << "\" ";
         ++i;
        }else{
            i = p.erase(i);
        }
    }
    std::cout << std::endl;
}