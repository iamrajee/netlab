import random
import matplotlib.pyplot as plt
class aloha():  
    ''' message generation prob. = q, message send prob. = p '''
    def __init__(self, nodes,p_send,q_generate,epochs):
        self.nodes = nodes
        self.p_send = p_send
        self.q_generate = q_generate
        self.epochs = epochs
        self.states = [False for i in range(self.nodes)]
        self.latencies = [0 for i in range(self.nodes)]
        self.result = []

    def message_generation(self):
        for i in range(self.nodes):
            if self.states[i] == False:
                if random.random()<=self.q_generate:
                    self.states[i] = True

    def transmission(self):
        senders = []
        actives = []
        for i in range(self.nodes):
            if self.states[i] == True:
                actives.append(i)
        for i in range(len(actives)):
            if random.random()<=self.p_send:
                senders.append(actives[i])
        if len(senders) > 1:
            self.result.append(False)
            for active in actives:
                self.latencies[active] += 1
        else:
            if (len(senders) == 0):
                self.result.append(True)
                for active in actives:
                    self.latencies[active] += 1
            else:
                self.states[senders[0]] = False
                actives.remove(senders[0])
                for active in actives:
                    self.latencies[active] +=1 
                self.result.append(True)

    def simulate(self):
        for i in range(self.epochs):
            self.message_generation()
            self.transmission()

n,p,q,e = 10,0.05,0.5,5000
accuracy,avg_latency,percentage_of_latents,plist = [],[],[],[]
while p<1:
	plist.append(p)
	x = aloha(n,p,q,e)
	x.simulate()
	accuracy.append(x.result.count(True)/e )
	latents = [latency for latency in x.latencies if latency!=0]
	avg_latency.append(sum(latents)//len(latents))
	p += 0.05

#print("states=",x.states)
#print("True count=",x.result.count(True))
#print("result=",x.result)
#print("latencies=",x.latencies)
#print("latents=",latents)
print("accuracy=",accuracy)
print("avg_latency=",avg_latency)

plt.plot(plist,accuracy)
plt.ylabel('p')
plt.ylabel('accuracy')
plt.show()
