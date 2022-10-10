#include <memory>
#include "num.hpp" // We use it for the variable name...
#include <list>

using student      = num;
using student_ref  = std::shared_ptr<student>;
using student_wref = std::weak_ptr<student>;
using population   = std::list<student_ref>;
using group        = std::list<student_wref>;

void kill(population& p, const std::string& target);
void print(group& g); // Not print(const group& g) ?!?!?

// In the comments, BAT(s, w) denotes the number s of shared pointer to
// batman and the number w of weak pointers to him.
// SUP(s, w) does the same for superman.

int main(int argc, char* argv[]) {
  population P;
  group      G1, G2, G3;
  {
    scope("Creating heroes...");
    student_ref batman {new student("Batman")}; // BAT(1, 0). Not the most efficient way... BAT(1, 0)
    auto superman       = std::make_shared<student>("Superman"); // SUP(1, 0)
    auto wonderwoman    = std::make_shared<student>("Wonder Woman");
    auto superdupont    = std::make_shared<student>("Super Dupont");
    auto casimir        = std::make_shared<student>("Casimir");
    auto electra        = std::make_shared<student>("Electra");
    rem("Building up population and groups");
    population heroes   = {batman, superman, wonderwoman, superdupont, electra}; // BAT(2, 0), SUP(2, 0). Yes, we have forgotten Casimir.
    group super_powered = {superman, wonderwoman, superdupont}; // SUP(2, 1)
    group female        = {wonderwoman, electra};
    group male          = {batman, superman, superdupont, casimir}; // BAT(2, 1), SUP(2, 2).
    rem("We affect local variables to main ones.");
    P  = heroes; // BAT(3, 1), SUP(3, 2). We could have moved (P = std::move(heroes)), but let us unefficiently copy.
    G1 = super_powered; // SUP(3, 3)
    G2 = female;
    G3 = male; // BAT(3, 2), SUP(3, 4)
    rem("Only Casimir is deleted here."); // as well as heroes, super_powered, female and male.
  }
  // BAT(1, 1), SUP(1, 2)
  {
    scope("Killing Wonder Woman"); // Yes, this can be done.
    kill(P, "Wonder Woman");
  }
  {
    scope("Group Status");
    print(G1); // "Superman" "Super Dupont"
    print(G2); // "Electra" 
    print(G3); // "Batman" "Superman" "Super Dupont"
  }
  return 0;
}

void kill(population& p, const std::string& target) {
  for(auto it = p.begin(); it != p.end(); ++it) {
    student_ref who = *it;
    if(std::string(*who) == target) {
      p.erase(it);
      return;
    }
  }
}

void print(group& g) {
  std::cout << scope_indent;
  // We clean up as we iterate.
  for(auto it = g.begin(); it != g.end(); /* nothing handeled here */)
    if(auto ref = it->lock(); ref) {/* thread safe, we get a shared from a weak... or nullptr */
      std::cout << '\"' << std::string(*ref) << "\" ";
      ++it;
    }
    else
      it = g.erase(it); // This is why g is not const...
  std::cout << std::endl;
}