#include "Graph.h"
#include <string>

int main()
{
    std::cout << "=============== Test default constructor and insert ================\n";
    gdwg::Graph<std::string, int> graph1{};
    
    graph1.addNode("Chunnan");
    graph1.addNode("Julie");
    graph1.addNode("Michael");
    graph1.addNode("Shuning");
    graph1.addNode("Brook");
    graph1.addNode("Kevin");

    graph1.addEdge("Chunnan", "Julie", 15);
    graph1.addEdge("Chunnan", "Michael", 16);
    graph1.addEdge("Chunnan", "Shuning", 17);
    graph1.addEdge("Chunnan", "Shuning", 10);
    graph1.addEdge("Shuning", "Brook", 6);
    graph1.addEdge("Shuning", "Kevin", 6);
    graph1.addEdge("Kevin", "Chunnan", 9);
    graph1.addEdge("Brook", "Chunnan", 4);


    graph1.printAll();
    
    std::cout << "=============== Test copy constructor ================\n";
    // test copy constructor
    gdwg::Graph<std::string, int> graph2{ graph1 };

    graph2.printAll();

    std::cout << "=============== Test copy assignment ================\n";
    gdwg::Graph<std::string, int> graph3{};

    graph3.addNode("Bill");
    graph3.addNode("Kate");
    graph3.addNode("Apple");
    graph3.addNode("Orange");
    graph3.addNode("Beef");
    graph3.addNode("Lamp");
    graph3.addNode("Jessica");

    graph3.addEdge("Bill", "Kate", 15);
    graph3.addEdge("Bill", "Apple", 95);
    graph3.addEdge("Bill", "Jessica", 15);
    graph3.addEdge("Bill", "Beef", 15);
    graph3.addEdge("Bill", "Orange", 15);
    graph3.addEdge("Bill", "Lamp", 15);

    graph3.printAll();

    graph2 = graph3;
    graph2.printAll();

    std::cout << "=============== Test move assignment ================\n";

    graph3 = std::move(graph1);
    graph1.printAll();
    graph3.printAll();

    graph1.addNode("Jupiter");
    graph1.addNode("Mars");
    graph1.addNode("Earth");
    graph1.printAll();

    std::cout << "=============== Test move constructor ================\n";

    gdwg::Graph<std::string, int> graph4{std::move(graph3)};
    graph4.printAll();

    graph3.addNode("China");
    graph3.addNode("Japan");
    graph3.addNode("Australia");
    graph3.printAll();

    std::cout << "================= Test replace ==================\n";

    graph4.replace("Brook", "LiXingYu");   
    graph4.printAll();

    graph4.replace("Chunnan", "Eric");
    graph4.printAll();

    std::cout << "================= Test delete edge ==================\n";

    gdwg::Graph<std::string, int> graph5{ graph4 };

    graph4.deleteEdge("Eric", "Shuning", 10);
    graph4.deleteEdge("Eric", "Shuning", 17);
    graph4.deleteEdge("LiXingYu", "Eric", 4);
    graph4.deleteEdge("Eric", "Michael", 16);
    graph4.deleteEdge("Shuning", "Kevin", 6);
    graph4.deleteEdge("Eric", "Julie", 15);
    graph4.deleteEdge("Shuning", "LiXingYu", 6);
    graph4.printAll();

    std::cout << "================= Test delete node ==================\n";

    gdwg::Graph<std::string, int> graph6{ graph5 };

    graph5.printAll();
    graph5.deleteNode("Michael");
    graph5.deleteNode("LiXingYu");
    graph5.deleteNode("Kevin");
    graph5.deleteNode("Julie");
    graph5.printAll();

    std::cout << "================= Test circle ==================\n";

    gdwg::Graph<std::string, int> graph7{ };
    graph7.addNode("Michael");
    graph7.addEdge("Michael", "Michael", 120);
    graph7.printAll();
    graph7.deleteNode("Michael");
    graph7.printAll();
    graph7.addNode("Jack");
    graph7.addEdge("Jack", "Jack", 30);
    graph7.printAll();
    graph7.deleteEdge("Jack", "Jack", 30);

    graph7.printAll();

    std::cout << "================ Test merge replace ===================\n";

    gdwg::Graph<std::string, int> graph8{ graph6 };
    graph6.addEdge("Eric", "Kevin", 6);
    graph6.addEdge("Eric", "Eric", 18);
    graph6.mergeReplace("Eric", "Shuning");
    graph6.printAll();

    std::cout << "=============== Test fake iterator print ================\n";

    for (graph8.begin(); !graph8.end(); graph8.next())
    {
        std::cout << graph8.value() << std::endl;
    }

    //graph6.isNode("Eric");
    graph6.printAll();
    //graph6.isConnected("Eric", "Kevin");
}