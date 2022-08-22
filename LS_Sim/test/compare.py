
import numpy as np
from root_operations import *

import matplotlib.pyplot as plt

import matplotlib as mpl
mpl.rcParams['font.size'] = 30

class CompareAna():
	def __init__(self , detsim_file):
		self.file_name = detsim_file;
		simtree,entries = readChain(files = [detsim_file], tree_name = "sim")
		opticks_simtree, entries = readChain(files = [detsim_file], tree_name = "opticks_sim")
		self.simtree = simtree
		self.opticks_simtree = opticks_simtree
	def init_branch(self,evtID = 0):
		'''
		copy the correspond event information to the member variable
		'''
		simtree = self.simtree
		opticks_simtree = self.opticks_simtree
		self.a_hittime = list(getBranchData(tree = simtree, entry = evtID, branch_name='hitTime'))
		self.b_hittime = list(getBranchData(tree = opticks_simtree, entry = evtID, branch_name='hitTime'))	
		self.a_posx = list(getBranchData(tree = simtree, entry = evtID, branch_name='GlobalPosX'))
		self.a_posy = list(getBranchData(tree = simtree, entry = evtID, branch_name='GlobalPosY'))
		self.a_posz = list(getBranchData(tree = simtree, entry = evtID, branch_name='GlobalPosZ'))

		self.b_posx = list(getBranchData(tree = opticks_simtree, entry = evtID, branch_name='GlobalPosX'))
		self.b_posy = list(getBranchData(tree = opticks_simtree, entry = evtID, branch_name='GlobalPosY'))
		self.b_posz = list(getBranchData(tree = opticks_simtree, entry = evtID, branch_name='GlobalPosZ'))

		
	def draw(self,option=""):
		idx = 0
		self.init_branch(evtID = idx)
		fig,ax = plt.subplots(figsize=(12,8))
		if option == "hittime":
			bins = np.linspace(0,1000,100)
			ax.hist(self.a_hittime,bins= bins, label = "Geant4" , histtype="step")
			ax.hist(self.b_hittime,bins= bins, label = "Opticks" , histtype="step")	
			ax.legend()
			ax.set_xlabel("time(ns)")
		if option == "posx":
			bins = np.linspace(0,20000,100)
			ax.hist(self.a_posx,bins= bins, label = "Geant4" , histtype="step")
			ax.hist(self.b_posx,bins= bins, label = "Opticks" , histtype="step")	
			ax.legend()
			ax.set_xlabel("x(mm)")
		if option == "posy":
			bins = np.linspace(0,20000,100)
			ax.hist(self.a_posy,bins= bins, label = "Geant4" , histtype="step")
			ax.hist(self.b_posy,bins= bins, label = "Opticks" , histtype="step")	
			ax.legend()
			ax.set_xlabel("y(mm)")
			
		if option == "posz":
			bins = np.linspace(0,20000,100)
			ax.hist(self.a_posz,bins= bins, label = "Geant4" , histtype="step")
			ax.hist(self.b_posz,bins= bins, label = "Opticks" , histtype="step")	
			ax.legend()
			ax.set_xlabel("z(mm)")
	

		ax.set_title("{} G4 vs Opticks".format(option))
		ax.set_ylabel("events")
		plt.show()	

if __name__ == "__main__":
		a = CompareAna("../detsim.root")
		a.draw(option = "hittime")
		a.draw(option = "posx")
		a.draw(option = "posy")
		a.draw(option = "posz")
	
