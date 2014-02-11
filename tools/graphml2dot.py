#!/usr/bin/env python
import lxml.etree as et
import sys

graphml = {
"graph": "{http://graphml.graphdrawing.org/xmlns}graph",
"node": "{http://graphml.graphdrawing.org/xmlns}node",
"edge": "{http://graphml.graphdrawing.org/xmlns}edge",
"data": "{http://graphml.graphdrawing.org/xmlns}data",
"label": "{http://graphml.graphdrawing.org/xmlns}data[@key='label']",
"edgeid": "{http://graphml.graphdrawing.org/xmlns}data[@key='edgeid']",
"interval": "{http://graphml.graphdrawing.org/xmlns}data[@key='interval']"
}

def display_usage():
    print "Usage: graphml2dot <filename>"
    print
    print "Convert graphs in GraphML format to Graphviz dot format."
    sys.exit(-1)


if __name__ == '__main__':

    if len(sys.argv) != 2:
        display_usage()

    tree = et.parse(sys.argv[1])
    root = tree.getroot()

    graph = tree.find(graphml.get("graph"))
    nodes = graph.findall(graphml.get("node"))
    edges = graph.findall(graphml.get("edge"))

    print "digraph {"

    comment = "/* This file is automatically converted from "+ sys.argv[1] + " by graphml2dot */"
    print comment
    print

    # convert nodes
    for n in nodes:
        node_attribs = {}
        print n.attrib['id'], 
        for data in n.findall(graphml.get('data')):
            node_attribs[data.get('key')] = data.text 
        str_node = '[label=' + node_attribs['label'] + ', id=' + n.attrib['id'] + ']'
        print str_node

    # convert edges
    for e in edges:
        edge_attribs = {}
        print e.attrib['source'], '->', e.attrib['target'],
        for data in e.findall(graphml.get('data')):
            edge_attribs[data.get('key')] = data.text 
        str_edge = '[label=' + edge_attribs['interval'] + ', interval=' + edge_attribs['interval'] + ']'
        print str_edge

    print "}"
