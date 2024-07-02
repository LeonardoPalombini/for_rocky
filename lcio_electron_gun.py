#####11##############################
#
# simple script to create lcio files with single particle
# events - modify as needed
# @author F.Gaede, DESY
# @date 1/07/2014
#
# initialize environment:
#  export PYTHONPATH=${LCIO}/src/python:${ROOTSYS}/lib
#
#####################################
import math
import random
from array import array

# --- LCIO dependencies ---
from pyLCIO import UTIL, EVENT, IMPL, IO, IOIMPL

#---- number of events ----------------------
nevt = 100000

outfile = "electronGun_1-100GeV_gen.slcio"

#--------------------------------------------


wrt = IOIMPL.LCFactory.getInstance().createLCWriter( )

wrt.open( outfile , EVENT.LCIO.WRITE_NEW ) 

random.seed()


#========== particle properties ===================

npart = 1

genstat  = 1

e_min = 1.
e_max = 100.

theta_min = 10./180.*math.pi
theta_max = 170./180.*math.pi

pdg = 11

mass =  0.
charge = -1.

decayLen = 1.e32 
#=================================================



for j in range( 0, nevt ):

    col = IMPL.LCCollectionVec( EVENT.LCIO.MCPARTICLE ) 
    evt = IMPL.LCEventImpl() 

    evt.setEventNumber( j ) 

    evt.addCollection( col , "MCParticle" )

    print (j, "-----------------------------")
    
    for ipart in range( 0, npart ):
    
        energy = random.uniform(e_min, e_max)
        theta = random.uniform(theta_min, theta_max) 
        phi =  random.random() * math.pi * 2.
        #theta = 0.5*math.pi
        #phi = 0.5*math.pi

        px = energy * math.cos( phi ) * math.sin( theta )
        py = energy * math.sin( phi ) * math.sin( theta )
        pz = energy * math.cos( theta )

        momentum  = array('f',[ px, py, pz ] )  

        epx = decayLen * math.cos( phi ) * math.sin( theta ) 
        epy = decayLen * math.sin( phi ) * math.sin( theta )
        epz = decayLen * math.cos( theta ) 

        endpoint = array('d',[ epx, epy, epz ] )  
        

#--------------- create MCParticle -------------------
        
        mcp = IMPL.MCParticleImpl() 

        mcp.setGeneratorStatus( genstat ) 
        mcp.setMass( mass )
        mcp.setPDG( pdg ) 
        mcp.setMomentum( momentum )
        mcp.setCharge( charge ) 

        if( decayLen < 1.e9 ) :   # arbitrary ...
            mcp.setEndpoint( endpoint ) 

        print ("  ", ipart, pdg, charge, energy, phi, theta)
    

#-------------------------------------------------------


        col.addElement( mcp )

        
    wrt.writeEvent( evt ) 


wrt.close() 
