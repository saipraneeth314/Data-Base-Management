#include <iostream>
#include <unordered_map>
#include <list>

using namespace std;

// Abstraction of a resource that can be locked. 
// A resource is locked in a 'mode' by a 'transaction'. 
// The lock request may be granted or put on wait based
// on a lock compatibility matrix. 

enum lockType 
{
  SHARED,
  EXCLUSIVE
};
enum lockStatus //(0-granted ,1-waiting)
{
  GRANTED,
  WAITING
};


class lockable_resource
{
  private:
    uint32_t txn_id_;
    lockType lock_type_; // SHARED, EXCLUSIVE
    lockStatus lock_status_; // GRANTED, WAITING
  public:
    lockable_resource(
      uint32_t txn_id,
      lockType lock_type,
      lockStatus lock_status):
      txn_id_(txn_id),
      lock_type_(lock_type),
      lock_status_(lock_status)
  {}
  lockType getLockType() 
  {
    return(lock_type_);
  }
  lockStatus getStatus()
  {
    return(lock_status_);
  }
  uint32_t getTxnId()
  {
    return(txn_id_);
  }
  void setLockStatus(lockStatus st)
  {
    lock_status_ = st;
  }
  void setLockType(lockType tp){
    lock_type_ = tp;
  }
};

// Resource name
// txnid
// type of lock

lockStatus lock(std::string resource_name, 
                std::uint32_t txn_id,
                lockType lock_type);

void unlock(std::string resource_name,
            std::uint32_t txn_id);

void upgrade(std::string resource_name,
            std::uint32_t txn_id);

void downgrade(std::string resource_name,
            std::uint32_t txn_id);

unordered_map<std::string, list<lockable_resource *>*> lock_table;

int main()
{ 
  cout <<"----------------lockStatus---------------------\n";
  cout << "----0->granted*****1->waiting-----\n";
  // bool ret = lock("AAA", 1234, lockType::EXCLUSIVE);
  // cout << ret << "\n";
  // bool ret = lock("AAA", 4567, lockType::EXCLUSIVE);
  // cout << ret << "\n";
  // bool ret = lock("AAA", 1234, lockType::SHARED);
  // cout << ret << "\n";
  //  ret = lock("AAA", 1234, lockType::SHARED);
  // cout << ret << "\n";
  bool ret = lock("AAB", 1234, lockType::SHARED);
  cout << ret << "\n";
  ret = lock("AAC", 1236, lockType::SHARED);
  cout << ret << "\n";

  // bool ret = lock("AAD", 4567, lockType::EXCLUSIVE);
  // cout << ret << "\n";
  ret = lock("AAA", 1235, lockType::SHARED);
  cout << ret << "\n";
  ret = lock("AAA", 1243, lockType::EXCLUSIVE);
  cout << ret << "\n";
  ret = lock("AAA", 12435, lockType::SHARED);
  cout << ret << "\n";
  ret = lock("AAA", 1245, lockType::SHARED);
  cout << ret << "\n";
  ret = lock("AAD", 124, lockType::SHARED);
  cout << ret << "\n";
  ret = lock("AAD", 1236, lockType::SHARED);
  cout << ret << "\n";
  ret = lock("AAE", 1237, lockType::SHARED);
  cout << ret << "\n";
  ret = lock("AAE", 1236, lockType::SHARED);
  cout << ret << "\n";


  upgrade("AAD", 1236);
  downgrade("AAD", 1236);

  cout<<"\n--------------unlockStatus-----------------\n";

  // unlock("AAA",1234);
  unlock("AAA",1243);
  unlock("AAC", 1236);
  unlock("AAD", 4567);
  unlock("AAE", 1237);
  unlock("AAE", 1236);
  unlock("AAF", 1237);//lockable resource is not present in lock_table


  return 0;

}


lockStatus lock(std::string resource_name,std::uint32_t txn_id,lockType lock_type)
{
  lockStatus retval = lockStatus::WAITING;
  // Check if lock exists. 
  //   No: Add to map, create new list and add lockable_resource to list
  if (lock_table.find(resource_name) == lock_table.end())
  {
    // lock table does not exist. 
    //  => locable resource has to be created. 
    //     list of lockable resources has to be created. 
    //     lock table shouldlst
    lockable_resource *lr = new lockable_resource(txn_id,lock_type,lockStatus::GRANTED);
    retval = lockStatus::GRANTED;
    list<lockable_resource*> *lst = new list<lockable_resource*>;
    lst->emplace_back(lr);
    lock_table[resource_name] = lst;
  }
  else
  { 
    list<lockable_resource*> *lst = lock_table[resource_name];
    auto iter = lst->rbegin();

    lockable_resource *lr = new lockable_resource(txn_id,lock_type,lockStatus::GRANTED);

    if((*(iter))->getLockType() == SHARED){
      if((lock_type == SHARED) && (((*(iter))->getStatus() == GRANTED))){//(S,G)-> (S,G) 
        retval = lockStatus::GRANTED;
      }
      else {//(S,W) -> (S/E,W) or (S,G)-> (E,W)(if lock_type is exclusive)
        lr->setLockStatus(lockStatus::WAITING);
        retval = lockStatus::WAITING;
      }
    }
    else if((*(iter))->getLockType() == EXCLUSIVE){ // (E,G/W) -> (S/E,W)
      lr->setLockStatus(lockStatus::WAITING);
      retval = lockStatus::WAITING;
    }
    lst->emplace_back(lr);
  }
  return(retval);
  // return values (0-granted ,1-waiting)
}

void unlock(std::string resource_name,std::uint32_t txn_id){
  
  if (lock_table.find(resource_name) == lock_table.end()){ //if no resource in the locktable
    cout<<"*****lockable resource is not present in lock_table*****\n";
    return;
  }

  list<lockable_resource*> *lst1 = lock_table[resource_name];
  int flag =0,f1=0;

  list<lockable_resource*>::iterator iter=lst1->begin();
  list<lockable_resource*>::iterator iter1;
  for(;iter!=lst1->end() ;iter++)
    if ((*(iter))->getTxnId()==txn_id){
      iter1 = next(iter);
      lock_table[resource_name]->remove(*(iter));
      cout <<"Unlocked transactionid:"<<txn_id<<"\n";
      f1=1;
      break;
    }
    if(f1==0){//if no transcation in the list
      cout<<"*****There is no transcation id in the list*****\n";
      return;
    }


  //after removing the given transaction and if there is no transaction left 
  // with the resource_name then deleting the resource_name from the lock table
  if(lock_table[resource_name]->empty()){
    lock_table.erase(resource_name); 
  }
  else{
    while(iter1!=lock_table[resource_name]->end()){ //checking next transcation of removed transcation if it is shared and waiting,now grant it. 
      if(((*(iter1))->getStatus()==lockStatus::WAITING)&&((*(iter1))->getLockType()==lockType::SHARED)){
        flag=1; // and set flag=1
        (*(iter1))->setLockStatus(lockStatus::GRANTED);
        cout<<"Next transcation lockStatus is set to GRANTED...\n";
        // cout<<"1\n";
        ++iter1;
      }
      else break;
    }
    // if flag is 0
    // if locktype is EXCLUSIVE and next transcation is not in the beginning means there transcations before it and there are shared and granted.
    if(flag==0){//if locktype is EXCLUSIVE and it is the 1st in the list lockStatus is set to Granted
      if(iter1==lock_table[resource_name]->begin()){
        (*(iter1))->setLockStatus(lockStatus::GRANTED);
        cout<<"Next txn is 1st txn in the list so lockStatus is set to GRANTED...\n";
        // cout<<"1\n";
        }
      }
    }

}


// Assume that Share should be granted.
// SG to upgrade EG and the remaning in the front are changed to SW and put at the back of the list
void upgrade(std::string resource_name, std::uint32_t txn_id) {
    auto& locks = *lock_table[resource_name];
    auto it = locks.begin();
    while (it != locks.end()) {
        auto& lock = **it;
        if (lock.getStatus() == lockStatus::WAITING) break;
        if (lock.getTxnId() != txn_id) {
            lock.setLockStatus(lockStatus::WAITING);
            auto it1 = it++;
            locks.splice(locks.end(), locks, it1);
        } else {
            lock.setLockType(lockType::EXCLUSIVE);
            std::cout << "upgraded\n";
            ++it;
        }
    }
  }

// EG to downgrade SG
// If there are multiple exclusive locks held by the same transaction, 
// this will only downgrade the first one it encounters in the list.
 
void downgrade(std::string resource_name, std::uint32_t txn_id) {
    auto& locks = *lock_table[resource_name];
    for (auto it = locks.begin(); it != locks.end(); ++it) {
        auto& lock = **it;
        if (lock.getTxnId() != txn_id) continue;
        if (lock.getLockType() == lockType::SHARED) return;
        lock.setLockType(lockType::SHARED);
        std::cout << "downgraded\n";
        return;
    }
}





