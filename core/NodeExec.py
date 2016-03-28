#
# CORE
# Copyright (c)2010-2011 the Boeing Company.
# See the LICENSE file included in this distribution.
#
# author: Jeff Ahrenholz <jeffrey.m.ahrenholz@boeing.com>
#
'''
nrl.py: defines services provided by NRL protolib tools hosted here:
http://cs.itd.nrl.navy.mil/products/
'''

from core.service import CoreService, addservice
#from core.misc.ipaddr import IPv4Prefix

cmdResult = []

class NrlService(CoreService):
    ''' Parent class for NRL services. Defines properties and methods
        common to NRL's routing daemons.
    '''
    _name = "NRLDaemon"
    _group = "GSUSensorWeb"
    _depends = ()
    _dirs = ()
    _configs = ()
    _startindex = 45
    _startup = ()
    _shutdown = ()

    @classmethod
    def generateconfig(cls,  node, filename, services):
        return ""

class NodeExec(NrlService):
    _name = "NodeExec"     
    _startup = ("/usr/lib/scoreplus/myservices/nodeexec ",)
    #_shutdown = ("killall batmand",)
        
    @classmethod
    def getstartup(cls,  node,  services):
        ''' Generate the appropriate command-line based on node interfaces.
        '''
        global cmdResult
        cmdResult=[]
        cmd = cls._startup[0]
        cmdResult.append(cmd + node.name)
        return (cmdResult)

    @classmethod
    def generateconfig(cls, node, filename, services):
        return ""
        
addservice(NodeExec)
