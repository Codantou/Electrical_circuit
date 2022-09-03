#include <iostream>
#include <string>         // std::string
#include <cstddef>         // std::size_t
#include <regex>
#include<string.h>

//#include "functions.hpp"


using namespace std;

/* I wanna write a script that takes a string and gives the transfer function. "RLC" means R, L and C in series
   "RLpCR1pL1" means RL series in parallel with (CR1 in series) in parallel with L1... AND SO ON
   1ST TASK: Reduce the number of components by making the equivalent impedance
   2nd TASK: Use voltage divider to get transfer function
   The last component(s) is/ are the output(s)

   In further and improved versions, we get also the differential equation of the voltage
*/

string impedance(string& str){
    string z;
    if (str.find("R") != std::string::npos) z = str;
    if (str.find("L") != std::string::npos) z = "j" + str + "w";
    if (str.find("C") != std::string::npos) z = "1/j" + str + "w";
    return z;

}

vector <string> get_all_series_comp(std::string s) {
    vector<string> t;
    std::regex reg("\\p");
    std::vector<std::string> out(
					std::sregex_token_iterator(s.begin(), s.end(), reg, -1),
					std::sregex_token_iterator()
					);

	for (auto &s: out) {
		//std::cout << s << std::endl;
        t.push_back(s);
	}
    return t;
}


/*string get_output(string& str){
    string outcomp;
    size_t found = str.find_last_of("p");  // find last occurrence of "p"
    outcomp = str.substr(found+1);    // takes substring from p onwards i.e the output circuit
    return outcomp;
}*/


vector<string> get_regex(std::string str, std::regex reg) {
    vector<string> t;
    // This holds the first match
    std::sregex_iterator currentMatch(str.begin(),
            str.end(), reg);
    
    // Used to determine if there are any more matches
    std::sregex_iterator lastMatch;
    
    // While the current match doesn't equal the last
    while(currentMatch != lastMatch){
        std::smatch match = *currentMatch;
        //std::cout << match.str() << "\n";
        t.push_back (match.str());
        currentMatch++;
    }
    //std::cout << std::endl;
    return t;
}

string Series_impedance(vector<string> t){
    string z;
    z = impedance(t[0]);
    for (int i = 1; i < t.size(); i++)
    {
        z = z + " + " + impedance(t[i]) ;
    }
    
    return z;
}

int main(){

    std::regex reg ("[RLC][0-9]");
    //string circuit1 = "R0L0pC0R1pL1R2C3";
    string circuit1;
    cout << "Veuillez entrer votre circuit : "; 
    cin >> circuit1;
    string z_eq;

    vector<string> t = get_all_series_comp(circuit1);
    string comp_out = t[t.size()-1];         //get the output component
    string z_out = Series_impedance(get_regex(comp_out, reg));
   
    cout << z_out << endl;
    //vector<string> comp_out = get_regex(get_output(circuit1), reg);
    
    for (int i = 0; i < t.size()-1; i++)
    {
        cout << t[i] << endl ;
    }
    cout << circuit1 << endl;
    //cout << comp_out << endl;
    string z1 = "1/(" + Series_impedance(get_regex(t[0], reg)) + ")";
    for ( int i = 1; i < t.size(); i++)
    {
        z1 = z1 + " + " + "1/(" + Series_impedance(get_regex(t[i], reg)) + ")";
        if (i == t.size()-1)
        {
            string str1 = t[i].substr(0, t[i].size()-2);      // REMOVE THE LAST COMPONENT BECAUSE IT'S THE OUTPUT
            z1 = z1 + " + " + "1/(" + Series_impedance(get_regex(str1, reg)) + ")";  //actually it's 1/z1
        }        
    }
    string str2 = comp_out.substr(comp_out.size()-2);
    z_eq = "1/[" + z1 + "] + " + impedance(str2);
    string transf = impedance(str2) + "/{ " + z_eq + " }";


    cout << z1 << endl; cout << z_eq << endl; 
    cout << "La fonction de transfert est : " << transf;
    
}