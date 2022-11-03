from erd import *
from table import *
from erd_converter import convert_to_table


erd4 = ERD(
    [Relationship('ShopsAt', [], [])],
    [EntitySet('Customer', ['customer_id', 'customer_name'], ['customer_id'], [('ShopsAt', Multiplicity.MANY)], [], []),
     EntitySet('Store', ['store_id', 'store_name'], ['store_id'], [('ShopsAt', Multiplicity.MANY)], [], [])])

db4 = Database([
    Table('Customer', set(['customer_id', 'customer_name']), set(
        ['customer_id']), set()),
    Table('Store', set(['store_id', 'store_name']), set(['store_id']), set()),
    Table('ShopsAt', set(['customer_id', 'store_id']), set(['customer_id', 'store_id']),
          set([(('customer_id',), 'Customer', ('customer_id',)), (('store_id',), 'Store', ('store_id',))]))])
erd7 = ERD(
    [Relationship('Prefers', [], [])],
    [EntitySet('Customer', ['customer_id', 'customer_name'], ['customer_id'], [('Prefers', Multiplicity.MANY)], [], []),
     EntitySet('Store', ['store_id', 'store_name'], ['store_id'], [('Prefers', Multiplicity.ONE)], [], [])])

db7 = Database([
    Table('Customer', set(['customer_id', 'customer_name', 'store_id']), set(['customer_id']),
          set([(('store_id',), 'Store', ('store_id',))])),
    Table('Store', set(['store_id', 'store_name']), set(['store_id']), set())])

erd8 = ERD(
    [Relationship('Prefers', ['since'], ['since'])],
    [EntitySet('Customer', ['customer_id', 'customer_name'], ['customer_id'], [('Prefers', Multiplicity.MANY)], [], []),
     EntitySet('Store', ['store_id', 'store_name'], ['store_id'], [('Prefers', Multiplicity.ONE)], [], [])])

db8 = Database([
    Table('Customer', set(['customer_id', 'customer_name']), set(
        ['customer_id']), set()),
    Table('Store', set(['store_id', 'store_name']), set(['store_id']), set()),
    Table('Prefers', set(['customer_id', 'store_id', 'since']), set(['customer_id', 'since']),
          set([(('customer_id',), 'Customer', ('customer_id',)), (('store_id',), 'Store', ('store_id',))]))])

erd9 = ERD(
    [Relationship('ManagerIsAnEmployee', [], [])],
    [EntitySet('Employee', ['employee_id', 'employee_name'], ['employee_id'], [('ManagerIsAnEmployee', Multiplicity.ONE)], [], []),
     EntitySet('Manager', [], [], [], ['Employee'], [])])

db9 = Database([
    Table('Employee', set(['employee_id', 'employee_name']), set(
        ['employee_id']), set()),
    Table('Manager', set(['employee_id']), set(['employee_id']),
          set([(('employee_id',), 'Employee', ('employee_id',))]))])

erd10 = ERD(
    [Relationship('FoundIn', [], [])],
    [EntitySet('Building', ['building_id', 'building_name'], ['building_id'], [('FoundIn', Multiplicity.ONE)], [], []),
     EntitySet('Room', ['room_number', 'max_capacity'], ['room_number'], [], [], ['FoundIn'])])

db10 = Database([
    Table('Building', set(['building_id', 'building_name']), set(
        ['building_id']), set()),
    Table('Room', set(['building_id', 'room_number', 'max_capacity']), set(['building_id', 'room_number']),
          set([(('building_id',), 'Building', ('building_id',))]))])


erd15 = ERD(
    [Relationship('PaintedBy', ['date_painted', ], []),
     Relationship('BornIn', ['birthdate', ], [])],
    [EntitySet('Painting', ['painting_name', ], ['painting_name', ], [('PaintedBy', Multiplicity.MANY)], [], []),
     EntitySet('Artist', ['artist_name', ], ['artist_name', ], [
         ('PaintedBy', Multiplicity.ONE), ('BornIn', Multiplicity.MANY)], [], []),
     EntitySet('City', ['city_name', ], ['city_name', ], [('BornIn', Multiplicity.ONE)], [], [])])

db15 = Database([
    Table('City', set(['city_name', ]), set(['city_name', ]), set()),
    Table('Artist', set(['artist_name', 'city_name', 'birthdate']), set(['artist_name', ]),
          set([(('city_name',), 'City', ('city_name',))])),
    Table('Painting', set(['painting_name', 'artist_name', 'date_painted']), set(['painting_name', ]),
          set([(('artist_name',), 'Artist', ('artist_name',))]))])
erd18 = ERD(
    [Relationship('Rel', [], [])],
    [EntitySet('R', ['a1', 'a2'], ['a1', ], [('Rel', Multiplicity.MANY)], [], []),
     EntitySet('S', ['b1', 'b2'], ['b2', ], [
         ('Rel', Multiplicity.ONE)], [], []),
     EntitySet('T', ['c1', 'c2'], ['c1', ], [('Rel', Multiplicity.ONE)], [], [])])
db18 = Database([
    Table('R', set(['a1', 'a2', 'b2', 'c1']), set(['a1', ]),
                set([(('b2',), 'S', ('b2',)), (('c1',), 'T', ('c1',))])),
    Table('S', set(['b1', 'b2']), set(['b2', ]), set()),
    Table('T', set(['c1', 'c2']), set(['c1', ]), set())])

erd19 = ERD(
    [Relationship('LabIsACourse', [], []),
     Relationship('VirtualIsALab', [], [])],
    [EntitySet('Course', ['crn', 'section'], ['crn', ], [('LabIsACourse', Multiplicity.ONE)], [], []),
     EntitySet('Lab', [], [], [
         ('VirtualIsALab', Multiplicity.ONE)], ['Course'], []),
     EntitySet('Virtual', [], [], [], ['Lab'], [])])

db19 = Database([
    Table('Course', set(['crn', 'section']), set(['crn', ]), set()),
    Table('Lab', set(['crn', ]), set(['crn', ]),
          set([(('crn',), 'Course', ('crn',))])),
    Table('Virtual', set(['crn', ]), set(['crn', ]), set([(('crn',), 'Lab', ('crn',))]))])


"""
m = convert_to_table(erd15)
print("\n".join([str(val) for val in m.tables]))
print("--------------------------------------")
print("\n".join([str(val) for val in db15.tables]))
print(m.__eq__(db4))
"""
