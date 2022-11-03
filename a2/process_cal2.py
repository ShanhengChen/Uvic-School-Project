#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
@author: shanhengchen :)
NETLINKID: V00946075

"""
import argparse
import re
from datetime import datetime


def is_root_label(line):
    """
    check if one line is a root label start
    :param line: one line from file
    :return: true if the line is a root label start
    """
    res = re.match('^<(.*)>(.*)<\/(.*)>$', line.strip())
    return res is None


def get_root_label_name(line):
    """
    get root label name
    :param line: one line from file
    :return: label name
    """
    res = re.match('^<(.*)>$', line.strip())
    res_groups = res.groups()
    return res_groups[0]


def get_label_name(line):
    """
    get label name
    :param line: one line from file
    :return: label name
    """
    res = re.match('^<(.*)>(.*)<\/(.*)>$', line.strip())
    res_groups = res.groups()
    return res_groups[0]


def get_label_data(line):
    """
    get label data
    :param line: one line from file
    :return: label data
    """
    res = re.match('^<(.*)>(.*)<\/(.*)>$', line.strip())
    res_groups = res.groups()
    return res_groups[1]


def parse_xml_file(file_name):
    """
    parse xml file
    :param file_name: xml file name
    :return: a dict contains that in xml file
    """
    with open(file_name) as file:
        lines = file.readlines()
    data = {}
    root_label_name = get_root_label_name(lines[0])
    data[root_label_name] = []
    sub_dict = {}
    sub_root_label_name = ""
    for line in lines[1:]:
        if is_root_label(line) and sub_root_label_name == "":
            sub_dict = {}
            sub_root_label_name = get_root_label_name(line)
            sub_dict[sub_root_label_name] = {}
        elif is_root_label(line) and sub_root_label_name != "":
            sub_root_label_name = ""
            data[root_label_name].append(sub_dict)
        else:
            label_name = get_label_name(line)
            label_data = get_label_data(line)
            sub_dict[sub_root_label_name][label_name] = label_data
    return data


def get_events_in_period(events, start_datetime, end_datetime):
    """
    get events in the period between start and end
    :param events: events in file
    :param start_datetime: start time
    :param end_datetime: end time
    :return: events in the period
    """
    event_list = [x['event'] for x in events['calendar']]
    events_in_period = []
    for event in event_list:
        event_datetime = datetime(year=int(event['year']), month=int(event['month']), day=int(event['day']))
        event['datetime'] = event_datetime
        if start_datetime < event_datetime < end_datetime:
            events_in_period.append(event)
    return sorted(events_in_period, key=lambda x: (x['datetime'], x['id']))


def get_data_from_circuits_by_id(circuits, id):
    """
    get data from circuits, filter by id
    :param circuits: circuits in file
    :param id: circuit id
    :return: circuit location, circuit name, time zone
    """
    location = ""
    name = ""
    timezone = ""
    circuit_list = [x['circuit'] for x in circuits['circuits']]
    for circuit in circuit_list:
        if circuit['id'] == id:
            location = circuit['location']
            name = f"{circuit['name']} ({circuit['direction']})"
            timezone = circuit['timezone']
            break
    return location, name, timezone


def get_data_from_broadcasters_by_id(broadcasters, id):
    """
    get data from broadcasters
    :param broadcasters: broadcasters in file
    :param id: id
    :return: broadcaster name
    """
    result = []
    id_list = id.split(',')
    broadcaster_list = [x['broadcaster'] for x in broadcasters['broadcasters']]
    for broadcaster in broadcaster_list:
        if broadcaster['id'] in id_list:
            result.append(broadcaster['name'])
    return result


def events_to_yaml(events):
    """
    convert events to yaml file
    :param events: events file
    :return:
    """
    lines = ["events:\n"]
    previous_date = ""

    for event in events:
        current_date = f"{event['day']}-{event['month']}-{event['year']}"
        event_datetime = datetime(
            year=int(event['year']),
            month=int(event['month']),
            day=int(event['day'])
        ).strftime("%A, %B %d, %Y")
        start_time = datetime.strptime(event['start'], "%H:%M").strftime("%I:%M %p")
        end_time = datetime.strptime(event['end'], "%H:%M").strftime("%I:%M %p")

        if current_date != previous_date:
            lines.append(f"  - {current_date}:\n")
            previous_date = current_date
        lines.append(f"    - id: {event['id']}\n")
        lines.append(f"      description: {event['description']}\n")
        lines.append(f"      circuit: {event['circuit']}\n")
        lines.append(f"      location: {event['location']}\n")
        lines.append(f"      when: {start_time} - {end_time} {event_datetime} ({event['timezone']})\n")
        lines.append(f"      broadcasters:\n")
        for x in event['broadcasters']:
            lines.append(f"        - {x}\n")
    with open("output.yaml", "w") as file:
        file.writelines(lines)


def main():
    """
    The main entry point for the program.
    notice: parser = argparse.ArgumentParser()
    get command line arguments 
    official doc link:https://docs.python.org/3/library/argparse.html
    """
    parser = argparse.ArgumentParser()
    parser.add_argument('--start')
    parser.add_argument('--end')
    parser.add_argument('--events')
    parser.add_argument('--circuits')
    parser.add_argument('--broadcasters')

    args = parser.parse_args()
    events = parse_xml_file(args.events)
    circuits = parse_xml_file(args.circuits)
    broadcasters = parse_xml_file(args.broadcasters)
    start_datetime = datetime.strptime(args.start, "%Y/%m/%d")
    end_datetime = datetime.strptime(args.end, "%Y/%m/%d")
    events_in_period = get_events_in_period(events, start_datetime, end_datetime)
    for i in range(len(events_in_period)):
        location, name, timezone = get_data_from_circuits_by_id(circuits, events_in_period[i]['location'])
        events_in_period[i]['location'] = location
        events_in_period[i]['circuit'] = name
        events_in_period[i]['broadcasters'] = get_data_from_broadcasters_by_id(broadcasters,
                                                                               events_in_period[i]['broadcaster'])
        events_in_period[i]['timezone'] = timezone
    events_to_yaml(events_in_period)


if __name__ == '__main__':
    main()
