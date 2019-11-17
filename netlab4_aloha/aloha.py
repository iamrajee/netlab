import random
class aloha():  
    '''
    Aloha network simulation. 
    Each node tries to send into a single channel. It generates a message (becomes active) 
    with probability q. Then it tries to send the message into the channel with probability
    p. If more than one nodes try to send, a collision occurs resulting to failure, otherwise 
    transmission is successful.
    '''

    def __init__(self, nodes,p_send,q_generate,epochs):
        # number of nodes
        self.nodes = nodes

        # probability that an active node will send the generated message
        self.p_send = p_send

        # probability that a node will generate a message
        self.q_generate = q_generate

        # number of time steps to simulate
        self.epochs = epochs

        # list with the state of each node, initially all inactive
        self.states = [False for i in range(self.nodes)]

        # list of latencies for each node 
        self.latencies = [0 for i in range(self.nodes)]

        # list of transmission outcomes. True for success
        self.result = []

    def message_generation(self):
        '''
        Helper function to check message generation
        '''
        for i in range(len(self.states)):
            # need only to check for inactive nodes
            if self.states[i] == False:
                if random.random()<=self.q_generate:
                    self.states[i] = True


    def transmission(self):
        senders = []
        actives = []

        # gather the indices of all active nodes
        for i in range(len(self.states)):
            if self.states[i] == True:
                actives.append(i)

        # check if an active node will try to send
        for i in range(len(actives)):
            if random.random()<=self.p_send:
                senders.append(actives[i])

        #print('Towards the end of this epoch the nodes that try to send are ' + str(len(senders)))
        #print(senders)
        # If more than one try to send we have a collision that results in transmission failure
        if len(senders) > 1:
            self.result.append(False)
            # so any active node experiences latency
            for active in actives:
                self.latencies[active] += 1
        else:
            # If none wants to send then certainly we don't experience failure
            if (len(senders) == 0):
                self.result.append(True)
                # but we might experience latency 
                for active in actives:
                    self.latencies[active] += 1
            else:
                # Success. Only one node tries to send
                self.states[senders[0]] = False
                # and all other active nodes experience latency again
                actives.remove(senders[0])
                for active in actives:
                    self.latencies[active] +=1 
                self.result.append(True)
                #print('Thus resulting in successful transmission.')



    def simulate(self):
        for i in range(self.epochs):
            #print('At the start of epoch number ' + str(i) + ' the states are')
            #print(self.states)
            self.message_generation()
            #print('At the middle of epoch number ' + str(i) + ' the states are')
            #print(self.states)
            self.transmission()
            #print('At the end of epoch number ' + str(i) + ' the states are')
            #print(self.states)


'''
Code to test the accuracy of the network (#successful_transfers/#epochs)
and latency for various values of p
'''
q=0.5
p=0.05
n=3
e=500
accuracy = []
avg_latency = []
percentage_of_latents = []
while p<1:
    x = aloha(n,p,q,e)
    x.simulate()
    accuracy.append( x.result.count(True)/e )
    latents = [latency for latency in x.latencies if latency!=0]
    avg_latency.append(sum(latents)/len(latents))
    p += 0.1
print(accuracy)
print(avg_latency)
