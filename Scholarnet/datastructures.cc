// Datastructures.cc
//
// Student name: Sari Myllynen
// Student email: sari.myllynen@tuni.fi
// Student number: 151127353

#include "datastructures.hh"

#include <random>

#include <cmath>
#include <stack>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)


Datastructures::Datastructures()
{
    Affiliations = {};
    Publications = {};
    AffiliationsByCoord = {};
}

Datastructures::~Datastructures()
{
    clear_all();
}

unsigned int Datastructures::get_affiliation_count()
{
    return Affiliations.size();
}

void Datastructures::clear_all()
{
    Publications.clear();
    Affiliations.clear();
    AffiliationsByCoord.clear();
    Connections.clear();
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    std::vector<AffiliationID> affiliationids = {};
    if(Affiliations.size()!=0)
    {
        // add AffiliationIDs one at a time into the temporary vector
        for(const auto& i:Affiliations)
        {
            affiliationids.push_back(i.second.ID);
        }
    }
    return affiliationids;
}

bool Datastructures::add_affiliation(AffiliationID id, const Name &name, Coord xy)
{
    Affiliation affiliation;

    auto idit = Affiliations.find(id);

    if(idit!=Affiliations.end())
    {
        return false; //affiliation ID already exists
    }

    affiliation.ID = id;
    affiliation.name = name;
    affiliation.location = xy;

    Affiliations[id]=affiliation;
    return true;
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    auto iditer = Affiliations.find(id);

    if(iditer != Affiliations.end())
    {
        return iditer->second.name;
    }
    return NO_NAME; //no such ID exists
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    auto iditer = Affiliations.find(id);

    if(iditer != Affiliations.end())
    {
        return iditer->second.location;
    }
    return NO_COORD; //no such ID exists
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{

    std::vector<const Affiliation*> temp_affs;
    temp_affs.reserve(Affiliations.size());

    for(const auto& aff: Affiliations)
    {
        const Affiliation* temp_pointer = &(aff.second);
        temp_affs.push_back(temp_pointer);
    }
    std::sort(temp_affs.begin(),temp_affs.end(),AffiliationNameComparison);

    std::vector<AffiliationID> organized_affs;
    organized_affs.reserve(temp_affs.size());

    for(const auto& aff :temp_affs)
    {
        organized_affs.push_back(aff->ID);
    }

    return organized_affs;
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{
    std::vector<const Affiliation*> temp_affs;
    temp_affs.reserve(Affiliations.size());
    std::vector<AffiliationID> organized_affs;


    for(const auto& aff: Affiliations)
    {
        const Affiliation* temp_pointer= &(aff.second);
        temp_affs.push_back(temp_pointer);

    }
    std::sort(temp_affs.begin(),temp_affs.end(),AffiliationCoordComparison);

    organized_affs.reserve(temp_affs.size());
    for(const auto& aff : temp_affs)
    {
        organized_affs.push_back((aff->ID));
    }

    return organized_affs;
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{//note to self- fix this before uploading to portfolio
  /*  auto readycoord = AffiliationsByCoord.find(xy);

    if (readycoord != AffiliationsByCoord.end())
    {
        return AffiliationsByCoord.at(xy);
    }
    else
    {*/
        auto coorditer = find_if(Affiliations.begin(),Affiliations.end(),
                              [xy](const std::pair<std::string,Affiliation>& found)
                             {return (found.second.location== xy);});
        if(coorditer != Affiliations.end())
        {
            // AffiliationsByCoord[coorditer->second.location] = coorditer->first;
            return coorditer->first;
        }

   // }
    return NO_AFFILIATION; //no affiliation with this coord found
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
   auto coorditer = Affiliations.find(id);

   if (coorditer != Affiliations.end())
   {
       // The old coordinate has to be removed from the secondary Affiliation map
       auto minicoorditer = AffiliationsByCoord.find(coorditer->second.location);
       if (minicoorditer != AffiliationsByCoord.end())
       {
           AffiliationsByCoord.erase(minicoorditer);
       }
       coorditer->second.location = newcoord;
       return true;
   }
   return false;
}

bool Datastructures::add_publication(PublicationID id, const Name &name, Year year, const std::vector<AffiliationID> &affiliations)
{
    Publication add_pub;

    auto pubid = Publications.find(id);

    if(pubid!=Publications.end())
    {
        return false; // Publication ID already exists
    }

    //create struct
    add_pub.ID = id;
    add_pub.title = name;
    add_pub.year = year;

    //add to the publications container
    Publications[id]=add_pub;

    for(const AffiliationID& affid : affiliations)
    {
        add_affiliation_to_publication(affid,id);
    }


    return true;
}

std::vector<PublicationID> Datastructures::all_publications()
{
    std::vector<PublicationID> temp_pubs;
    for(const auto& pub:Publications)
    {
        temp_pubs.push_back(pub.second.ID);
    }

    return temp_pubs;
}

Name Datastructures::get_publication_name(PublicationID id)
{
    auto nameiter = Publications.find(id);

    if(nameiter!=Publications.end())
    {

        return nameiter->second.title;
    }

    return NO_NAME;
}

Year Datastructures::get_publication_year(PublicationID id)
{
    auto yeariter = Publications.find(id);
    if(yeariter!=Publications.end())
    {

        return yeariter->second.year;
    }

    return NO_YEAR;
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
   std::vector<AffiliationID> aff_list;

   auto pubiter = Publications.find(id);

   if(pubiter!=Publications.end())
   {
       for(const auto& i: pubiter->second.published_by)
       {
            aff_list.push_back(i);
        }
   }
   else
   {
       aff_list.push_back(NO_AFFILIATION); // given ID was not found
   }

   return aff_list;
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    auto refiter = Publications.find(id);
    auto refeeiter = Publications.find(parentid);

    Publication* child_ptr = nullptr;
    Publication* parent_ptr = nullptr;

    if(refiter != Publications.end() && refeeiter != Publications.end())
    {

        child_ptr = &(refiter->second);
        parent_ptr = &(refeeiter->second);

        refiter->second.referenced_by = parent_ptr;
        refeeiter->second.references.insert(child_ptr);

        return true;
    }

    return false;
}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    std::vector<PublicationID> temp_pubs = {};
    auto pubiter = Publications.find(id);

    if(pubiter!=Publications.end())
    {
        for(const auto& i : pubiter->second.references)
        {
            temp_pubs.push_back(i->ID);
        }
    }

    return temp_pubs;
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid,
                                                    PublicationID publicationid)
{

    auto pubiter = Publications.find(publicationid);
    auto affiter = Affiliations.find(affiliationid);

    if(pubiter != Publications.end() && affiter != Affiliations.end())
    {
        Publication* pub_ptr = &(pubiter->second);

        pubiter->second.published_by.insert(affiter->second.ID);
        affiter->second.aff_Publications.insert(pub_ptr);

        if(pubiter->second.published_by.size()>1)
        {
            for(const AffiliationID& i : pubiter->second.published_by)
            {
                AddConnection(affiliationid,i);

            }
        }

        return true;
    }

    return false;
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
    std::vector<PublicationID> pubs;

    auto affiter = Affiliations.find(id);

    if(affiter != Affiliations.end())
    {

        for(const auto& i: affiter->second.aff_Publications)
        {
            pubs.push_back(i->ID);
        }
    }
    else
    {
        pubs.push_back(NO_PUBLICATION); //if given AffiliationID was not found
    }

    return pubs;
}

PublicationID Datastructures::get_parent(PublicationID id)
{
    auto pubiter = Publications.find(id);

    if(pubiter!=Publications.end() && pubiter->second.referenced_by!=nullptr)
    {
        return pubiter->second.referenced_by->ID;
    }

    return NO_PUBLICATION; //PublicationID does not exist
}

std::vector<std::pair<Year, PublicationID> > Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{
    std::vector<std::pair<Year,PublicationID>> returnvec;

    auto affiter = Affiliations.find(affiliationid);

    if(affiter != Affiliations.end())
    {
        for(const auto& pubs : affiter->second.aff_Publications)
        {
            if(pubs->year >= year)
            {
                std::pair<Year, PublicationID> pair(pubs->year,pubs->ID);

                returnvec.push_back(pair);
            }
        }
    }
    else
    {
        // given AffiliationID does not exist
        std::pair<Year, PublicationID> pair(NO_YEAR,NO_PUBLICATION);
        returnvec.push_back(pair);
    }

    sort(returnvec.begin(),returnvec.end(),PublicationYearComparison);

    return returnvec;
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    std::vector<PublicationID> referencechain;

    auto pubid = Publications.find(id);
    if(pubid !=Publications.end())
    {
        Publication* ptr =&(pubid->second);

        referencechain = FindReferenceChain(ptr);
    }
    else
    {
        referencechain.push_back(NO_PUBLICATION); // Given PublicationID does not exist
    }
    return referencechain;
}

std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{
    std::vector<PublicationID> ref_list;
    auto pubiter = Publications.find(id);

    if(pubiter != Publications.end())
    {

        Publication *pubptr =&(pubiter->second);
        FindRefereeChain(ref_list,pubptr);

    }
    else
    {
        ref_list.push_back(NO_PUBLICATION);
    }

    return ref_list;

}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord /*xy*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_closest_common_parent()");
}

bool Datastructures::remove_affiliation(AffiliationID id)
{
    auto removeiter = Affiliations.find(id);

    if(removeiter != Affiliations.end())
    {
        //do removals
        for(auto i : removeiter->second.aff_Publications)
        {
            i->published_by.erase(removeiter->second.ID);
        }
        removeiter->second.aff_Publications.clear();
        //this one only after pointer to it has been deleted everywhere
        Affiliations.erase(removeiter);
        return true;
    }
    else
    {
        return false; // no such id exists, nothing will be deleted
    }
}

PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2)
{
    auto pubiter1 = Publications.find(id1);
    auto pubiter2 = Publications.find(id2);
    if(pubiter1 != Publications.end() && pubiter2 != Publications.end())
    {
        std::vector<PublicationID> chain1 = FindReferenceChain(&(pubiter1->second));
        std::vector<PublicationID> chain2 = FindReferenceChain(&(pubiter2->second));
        auto iter = std::find_first_of(chain1.begin(),chain1.end(),
                                       chain2.begin(),chain2.end());
        if(iter != chain1.end())
        {
            return *iter;
        }

    }

    return NO_PUBLICATION;

}

bool Datastructures::remove_publication(PublicationID publicationid)
{
    auto puberase = Publications.find(publicationid);
    if(puberase != Publications.end())
    {
        Publication* pointerase = &(Publications.at(publicationid));

        //remove the publication from all affiliations
        for(auto &i: puberase->second.published_by)
        {
            Affiliations.at(i).aff_Publications.erase(pointerase);
        }
        //Then the fun part, removing it from all the reference chains
        if(puberase->second.referenced_by)
        {
            puberase->second.referenced_by->references.erase(pointerase);
        }
        for(auto &i: puberase->second.references)
        {
            i->referenced_by=nullptr;
        }
        //this is to be done last
        Publications.erase(puberase);
        return true;
    }
    else
    {
        return false; //no given id exists
    }
}

std::vector<Connection> Datastructures::get_connected_affiliations(AffiliationID id)
{

    std::vector<Connection> return_connection;

    auto iditer = Affiliations.find(id);

    if(iditer != Affiliations.end())    
      {

        for(const auto& i : iditer->second.aff_Connections)
        {

            auto iiter = std::find(Connections.begin(),Connections.end(),*i.second);


            if(iiter->aff1==id){

                return_connection.push_back(*iiter);

            }
            else
            {
                Connection mirror;
                mirror.aff1=iiter->aff2;
                mirror.aff2= iiter->aff1;
                mirror.weight = iiter->weight;
                return_connection.push_back(mirror);
            }

            }

        }

    return return_connection;

}

std::vector<Connection> Datastructures::get_all_connections()
{
   // std::cerr<<"get_all_connections"<<std::endl;
    std::vector<Connection> con_vec;
    for(const auto& i : Connections)
    {
        con_vec.push_back(i);
    }

    return con_vec;
}

Path Datastructures::get_any_path(AffiliationID source, AffiliationID target)
{
    Path returnpath;
/*
    std::set<AffiliationID> visited;

    returnpath = FindRoute(visited,source,target);
    std::reverse(returnpath.begin(),returnpath.end()); */


        std::stack<AffiliationID> DFSstack;
        std::map<AffiliationID,NodeState> colourmap;


        for(const auto& i: Affiliations)
        {

            colourmap[i.first].nodecolour=WHITE;

        }

        auto sourceiter = Affiliations.find(source);
        auto targetiter = Affiliations.find(target);
        if(sourceiter != Affiliations.end() && targetiter != Affiliations.end())
        {
            DFSstack.push(sourceiter->first);
            while(DFSstack.size() != 0)
            {
                AffiliationID u = DFSstack.top();
                DFSstack.pop();

                if(colourmap[u].nodecolour== WHITE)
                {

                    colourmap[u].nodecolour = GREY;
                    DFSstack.push(u);

                    for(const auto& v : Affiliations[u].aff_Connections)
                    {
                        if(v.first==target)
                        {
                            // here we need actions to do when we find the target
                        }
                        if(colourmap[v.first].nodecolour==WHITE)
                        {
                            DFSstack.push(v.first);
                            colourmap[v.first].pi = u;

                        }
                        else if(colourmap[v.first].nodecolour==GREY)
                        {

                        }
                    }

                }
                else
                {
                    colourmap[u].nodecolour=BLACK;
                }


            }
        }
    return returnpath;

}

Path Datastructures::get_path_with_least_affiliations(AffiliationID /*source*/, AffiliationID /*target*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_path_with_least_affiliations()");
}

Path Datastructures::get_path_of_least_friction(AffiliationID /*source*/, AffiliationID /*target*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_path_of_least_friction()");
}

PathWithDist Datastructures::get_shortest_path(AffiliationID /*source*/, AffiliationID /*target*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_shortest_path()");
}

bool Datastructures::AffiliationNameComparison(const Affiliation* a, const Affiliation* b)
{
    Name lowerA = a->name, lowerB= b->name;
    std::transform(lowerA.begin(),lowerA.begin()+1,lowerA.begin(),::tolower);
    std::transform(lowerB.begin(),lowerB.begin()+1,lowerB.begin(),::tolower);
    return lowerA < lowerB;
}

bool Datastructures::AffiliationCoordComparison(const Affiliation *a, const Affiliation *b)
{
    float sqrta = sqrt(pow(a->location.x,2)+pow(a->location.y,2));
    float sqrtb = sqrt(pow(b->location.x,2)+pow(b->location.y,2));

    if(sqrta == sqrtb)
    {
        return a->location.y < b->location.y;
    }
    return sqrta < sqrtb;
}

bool Datastructures::PublicationYearComparison(const std::pair<Year, PublicationID> a, const std::pair<Year, PublicationID> b)
{
    if(a.first==b.first)
    {
        return a.second < b.second;
    }
    return a.first < b.first;
}

std::vector<PublicationID> Datastructures::FindReferenceChain(const Publication* ptr)
{
    std::vector<PublicationID> refchain;
    const Publication* pub_ptr = ptr;

    while(pub_ptr != nullptr)
    {
        if(pub_ptr->referenced_by != nullptr)
        {
            refchain.push_back(pub_ptr->referenced_by->ID);
        }
        pub_ptr = pub_ptr->referenced_by;
    }
    return refchain;
}

void Datastructures::FindRefereeChain(std::vector<PublicationID> &pubs,const Publication* ptr)
{
    for(Publication* i : ptr->references)
    {
        pubs.push_back(i->ID);
        FindRefereeChain(pubs,i);
    }
}

void Datastructures::AddConnection(AffiliationID aff1, AffiliationID aff2)
{


    if(aff1!=aff2){
        auto aff1iter = Affiliations.find(aff1);
        auto aff2in1iter = aff1iter->second.aff_Connections.find(aff2);

        if(aff2in1iter != aff1iter->second.aff_Connections.end())
        {
            aff2in1iter -> second->weight +=1;
        }

        else
        {
            Connection create;
            if(aff1<aff2)
            {
                create.aff1 = aff1;
                create.aff2 = aff2;
            }
            else if(aff1>aff2)
            {
                create.aff1 = aff2;
                create.aff2 = aff1;
            }
            create.weight = 1;

            Connections.push_back(create);

            aff1iter->second.aff_Connections[aff2]=&(Connections.back());
            Affiliations.find(aff2)->second.aff_Connections[aff1]=&(Connections.back());

        }
    }
}

Path Datastructures::FindRoute(std::set<AffiliationID>& visited, AffiliationID current, AffiliationID target)
{

    Path returnvec;
    if(visited.find(current)!=visited.end())
    {

        return returnvec;
    }
    visited.insert(current);

    for(const auto& i : Affiliations[current].aff_Connections)
    {
        if(i.first == target)
        {
          /*  if(i.second->aff1 != current)
            {

                Connection retcon;
                retcon.aff1 = i.second->aff2;
                retcon.aff2 = i.second->aff1;
                retcon.weight=i.second->weight;
                returnvec.push_back(retcon);
            }
            else
            {*/
                 returnvec.push_back(*i.second);
          //  }

            return returnvec;
        }
        else
        {
            returnvec = FindRoute(visited, i.first, target);
            if(returnvec.size()!=0)
            {
/*
                if(i.second->aff1 != current)
                {
                    Connection retcon;
                    retcon.aff1 = i.second->aff2;
                    retcon.aff2 = i.second->aff1;
                    retcon.weight=i.second->weight;

                    returnvec.push_back(retcon);
                }

                else
                {*/
                    returnvec.push_back(*i.second);
                //}

                return returnvec;
            }
        }
    }

    return returnvec;
}
