from erd import *
from table import *

def convert_to_table( erd ):
    db = Database([])
    for ele in erd.entity_sets:  #for entity-set 
        if len(ele.parents) == 0: # erd.entity_sets has no parents relation 
            m = Table(ele.name,set(ele.attributes),set(ele.primary_key),set())
            
        else:   # handle parent relation (since we only have a parent we do not consider parents cases)
            for parent in erd.entity_sets: #find the parent relation 
                parent_list = []
                if [parent.name] == ele.parents:
                    parent_list = parent.primary_key
                    parent_parent = parent.parents
                    break
            temp_parent_foreign_key_list = []
            if len(parent_list) != 0:
                if len(parent_parent) == 0: #make sure the entity set is not from parent parent
                    temp_parent_foreign_key_list = transfer_fk(parent_list, parent.name)
            else:
                for new in db.tables:
                    if new.name == parent.name:
                        temp_parent_foreign_key_list = transfer_fk(new.primary_key,new.name)
                        break
            m = Table(ele.name,set(parent_list),set(parent_list),set(temp_parent_foreign_key_list))
        db.tables.append(m)

    for ele in erd.relationships:#for relationship
        m = Table(ele.name,set(ele.attributes),set(ele.primary_key),set())
        temp_name = ele.name
        temp_value = 0
        #search for relation and calculate what relation it is 
        #many-many = 2
        #one -many = 1  two situation 
        #one -one  = 0  or weak entity
        for rel in erd.entity_sets:
            if len(rel.connections) == 0: # no relationships involving then skip
                continue
            for calculate in rel.connections:
                if calculate[0] == temp_name: #find entity_set which relative to rn
                    temp_value += calculate[1].value

        if temp_value == 0: # wean entity set occur (no extra relation appear)
            for weak in erd.entity_sets:    #find the relation need support
                if weak.supporting_relations == [ele.name]:
                    break
            for weak_table in db.tables:    # identify weak entity set table in database
                if weak_table.name == weak.name:
                    break
            for sup in erd.entity_sets: #find the supporting relation
                if sup.connections[0][0] == ele.name and sup.name != weak.name:
                    break
            weak_table.attributes.update(set(sup.primary_key))
            weak_table.primary_key.update(set(sup.primary_key))
            temp_weak_foreign_list = transfer_fk(sup.primary_key,sup.name)
            if temp_weak_foreign_list != []:
                weak_table.foreign_keys.update(temp_weak_foreign_list)
     
        if temp_value == 1: #always put primary key of one relation side into attributes of many relation side and ref as foreign key
            for temp_many_relation_set in erd.entity_sets:
                for temp_many_relation in  temp_many_relation_set.connections:
                    if temp_many_relation[0] == ele.name and temp_many_relation[1].name == "MANY":
                        many_relation_set = temp_many_relation_set
                    
            stop = 0
            for temp_one_relation_set in erd.entity_sets:
                if stop != 1:
                    for temp_one_relation in temp_one_relation_set.connections:
                        if temp_one_relation[0] == ele.name and temp_one_relation[1].name == "ONE":
                                one_relation_set = temp_one_relation_set
                                stop +=1

            temp_many_foreign_list = transfer_fk(one_relation_set.primary_key,one_relation_set.name)
            if len(ele.primary_key) != 0:        # relationship has attributes so need one more table
                combine_attributes_of_list = []
                combine_attributes_of_list.extend(many_relation_set.primary_key)
                combine_attributes_of_list.extend(one_relation_set.primary_key)
                m.attributes.update(set(combine_attributes_of_list))
                m.primary_key.update(set(many_relation_set.primary_key))
                ttemp_many_foreign_list = transfer_fk(many_relation_set.primary_key,many_relation_set.name)
                m.foreign_keys.update(set(ttemp_many_foreign_list))
                m.foreign_keys.update(set(temp_many_foreign_list))
                db.tables.append(m)
            
            if len(ele.primary_key) == 0:   #Relationship does not have attributes so no extra relation 
                for table in db.tables:
                    if table.name == many_relation_set.name: #find the many relation table
                        table.attributes.update(set(one_relation_set.primary_key))
                        if len(ele.attributes) !=0:
                            table.attributes.update(set(ele.attributes))
                        table.foreign_keys.update(set(temp_many_foreign_list))
                        for temp_one_relation_set in erd.entity_sets:                   #SEARCH AGAIN GET DEAL MANY ONE ONE 
                            for temp_one_relation in temp_one_relation_set.connections:
                                if temp_one_relation[0] == ele.name and temp_one_relation[1].name == "ONE" and temp_one_relation != one_relation_set:
                                        another = temp_one_relation_set
                        table.attributes.update(set(another.primary_key))
                        table.foreign_keys.update(set(transfer_fk(another.primary_key,another.name)))
 
                

        if temp_value == 2:
            ele_attributes_list = ele.attributes
            m.attributes.clear()
            temp_namelist = set()
            for rel in erd.entity_sets:
                if len(rel.connections) == 0: # no relationships involving then skip
                    continue
                for relation in rel.connections:
                    if relation[0] == temp_name:
                        temp_namelist.update(set(rel.primary_key))
                        m.foreign_keys.update(set(transfer_fk(rel.primary_key,rel.name)))

            m.attributes.update(temp_namelist)
            m.attributes.update(ele_attributes_list)
            m.primary_key.update(temp_namelist)
            db.tables.append(m)
    return db

def transfer_fk(primary_key,name):
    if len(primary_key) == 0:
        return []
    temp_primary_key_tuple= tuple(primary_key)
    temp_foreign_tuple = (temp_primary_key_tuple,name,temp_primary_key_tuple)
    temp_foreign_list= [temp_foreign_tuple]
    return temp_foreign_list
