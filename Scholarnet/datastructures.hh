// Datastructures.hh
//
// This project has been made for Data structures & algorithms course
// at uni, and it has been built on top of a given template. The parameters
// and return values for the functions were all defined in the assignment
// description, a few helper functions has been created on top of these.
// Furthermore, everything before the beginning of the Class "Datastructures"
// was given for the task.

// The purpose of this program is to store information about universities-
// affiliations, and the publications they've published.

// However, this project is not fully finished and I will keep refining
// and updating it as I have time

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <set>
#include <list>

// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
struct Connection;
// Type for a distance (in arbitrary units)
using Distance = int;

using Path = std::vector<Connection>;
using PathWithDist = std::vector<std::pair<Connection,Distance>>;

// Return values for cases where required thing was not found
AffiliationID const NO_AFFILIATION = "---";
PublicationID const NO_PUBLICATION = -1;
Name const NO_NAME = "!NO_NAME!";
Year const NO_YEAR = -1;
Weight const NO_WEIGHT = -1;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};


// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

struct Connection
{
    AffiliationID aff1 = NO_AFFILIATION;
    AffiliationID aff2 = NO_AFFILIATION;
    Weight weight = NO_WEIGHT;
    bool operator==(const Connection& c1) const{
        return (aff1==c1.aff1) && (aff2==c1.aff2) && (weight==c1.weight);
    }
};
const Connection NO_CONNECTION{NO_AFFILIATION,NO_AFFILIATION,NO_WEIGHT};


// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Returns the # of affiliations
    unsigned int get_affiliation_count();

    // Clears everything, tabula rasa.
    void clear_all();

    // Returns a vector of all affiliations
    std::vector<AffiliationID> get_all_affiliations();

    // Adds a new affiliation
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Returns the name of the affiliation with given id
    Name get_affiliation_name(AffiliationID id);

    // Returns the coordinates of the affiliation with given id
    Coord get_affiliation_coord(AffiliationID id);

    // returns a vector of all affiliations arranged alphabetically
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Returns a vector of all affiliations arranged by increasing distance from 0 0
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Finds affiliation with given coordinates
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Changes affiliation's coordinates
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    //Adds a new publication to specified affiliationID
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Returns a vector of all Publications
    std::vector<PublicationID> all_publications();

    // Returns the name of the publication with given ID
    Name get_publication_name(PublicationID id);

    // Returns the year the publication was published
    Year get_publication_year(PublicationID id);

    // Returns a vector of all affiliationIDs affiliated with given publication ID
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // adds a reference link between two publications
    bool add_reference(PublicationID id, PublicationID parentid);

    // Returns a vector of publicationIDs that the given publication directly references
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Links the given publication to a specific affiliation
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Returns a vector containing all publications related to given affiliation
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Returns the given publication's parent publication
    PublicationID get_parent(PublicationID id);

    // For given affiliation, returns all publications published after given year
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Follows the chain of referencing publications to the root - ie . until no more publications refer to the chain
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);

    // Same as previous but opposite
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Not implemented - Returns 3 closest affiliations to the given coordinate
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Removes all evidence that the given affiliation ever existed
    bool remove_affiliation(AffiliationID id);

    // Compares two publications, returns their closest common parent if one exists
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Removes publication from existence
    bool remove_publication(PublicationID publicationid);

    // Returns all affiliations connected with the given one
    std::vector<Connection> get_connected_affiliations(AffiliationID id);

    // Shows all existing connections
    std::vector<Connection> get_all_connections();

    // Not finished - Returns a path between to affiliations if one exists
    Path get_any_path(AffiliationID source, AffiliationID target);

    // Not implemented yet
    Path get_path_with_least_affiliations(AffiliationID source, AffiliationID target);

    // Not implemented yet
    Path get_path_of_least_friction(AffiliationID source, AffiliationID target);

    // Not implemented yet
    PathWithDist get_shortest_path(AffiliationID source, AffiliationID target);


private:
    // this enumerator is used for pathfinding
    enum NodeVisit{WHITE,GREY,BLACK};

    struct Publication{
        // These are the compulsory properties
        PublicationID ID;
        Name title;
        Year year;

        // Vector of AffIDs that have partaken in publishing the paper
        std::unordered_set<AffiliationID> published_by;
        // pointer to the Publication that uses this publication as reference
        Publication* referenced_by = nullptr;
        // Vector for pointers to other Publications this Publication references
        std::unordered_set<Publication*> references = {};
    };

    struct Affiliation{
        // Again, the compulsory properties
        AffiliationID ID;
        Name name;
        Coord location;

        //vector that stores pointers to all the publications this Affiliation is
        // part of
        std::unordered_set<Publication*> aff_Publications;

        //save all the connections to this
        std::map<AffiliationID, Connection*> aff_Connections;

    };
    struct NodeState{
        // struct to help store information on nodes along a path
        NodeVisit nodecolour;
        AffiliationID pi=NO_AFFILIATION;
        Weight cost = INT32_MAX;

    };

    // to compare Affiliations by their name, case-sensitive:
    static bool AffiliationNameComparison(const Affiliation* a, const Affiliation* b);

    // This structure stores all the affiliations
    std::map<AffiliationID,Affiliation> Affiliations;

    // To compare Affiliations by their location:
    static bool AffiliationCoordComparison(const Affiliation* a, const Affiliation* b);

    // This structure stores all the publications
    std::map<PublicationID,Publication> Publications;

    // This map saves AffiliationIDs by their coordinates, new one is created when find_affiliation_with_coord is called
    std::unordered_map<Coord,AffiliationID, CoordHash> AffiliationsByCoord;

    // compares publications years, if same, compares IDs
    static bool PublicationYearComparison(const std::pair<Year, PublicationID> a,
                                          const std::pair<Year, PublicationID> b);

    //finds a chains of reference linked to the Publication given as parameter
    std::vector<PublicationID>FindReferenceChain(const Publication* ptr);

    // recursive function for finding the referee chains
    void FindRefereeChain(std::vector<PublicationID> &pubs, const Publication* ptr);

    // hopefully stores the connections
    std::list<Connection> Connections;

    // Adds a new connection between two affiliations
    void AddConnection(AffiliationID aff1, AffiliationID aff2);

    // Finds a Path between two affiliations. Works fine with small datasets, but experiences stackoverflow if N -> 100 000
    Path FindRoute(std::set<AffiliationID>& visited, AffiliationID current, AffiliationID target);
};

#endif // DATASTRUCTURES_HH
