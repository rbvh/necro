
import dataclasses
from dataclasses import dataclass, field
from typing import List
import random

@dataclass 
class BoardState:
    black: int = 0
    redgreen: int = 0
    blue: int = 0
    landdrops: int = 1
    cards: List[int] = field(default_factory=list)
    bargain: int = 0
    win: bool = False

    def __repr__(self):
        card_names = [card.__name__ if isinstance(card, type) else card.__class__.__name__ for card in self.cards]

        return f"BoardState(black={self.black}, redgreen={self.redgreen}, blue={self.blue}, landdrops={self.landdrops}, cards={card_names}, bargain={self.bargain}, win={self.win})"

class Card:
    color = None
    def __init__(self):
        pass

class DarkRitual(Card):
    color = 'black'
    def act(state):
        out = []
        if state.black > 0:
            out += [dataclasses.replace(state, black=state.black+2)]
        return out
    
class CabalRitual(Card):
    color = 'black'
    def act(state):
        out = []
        if state.black > 1:
            out += [dataclasses.replace(state, black=state.black+1)]
        if state.black > 0 and state.redgreen > 0:
            out += [dataclasses.replace(state, black=state.black+2, redgreen=state.redgreen-1)]
        if state.black > 0 and state.blue > 0:
            out += [dataclasses.replace(state, black=state.black+2, blue=state.blue-1)]
        return out

class SpiritGuide(Card):
    color = 'redgreen'
    def act(state):
        return [dataclasses.replace(state, redgreen=state.redgreen+1)]
    
class VaultOfWhispers(Card):
    def act(state):
        out = []
        if state.landdrops > 0:
            out += [dataclasses.replace(state, black=state.black+1, landdrops=state.landdrops-1, bargain=state.bargain+1)]
        return out
        
class GemstoneMine(Card):
    def act(state):
        out = []
        if state.landdrops > 0:
            out += [dataclasses.replace(state, redgreen=state.redgreen+1, landdrops=state.landdrops-1),
                    dataclasses.replace(state, black=state.black+1,       landdrops=state.landdrops-1)]
        return out

class Valakut(Card):
    color = 'redgreen'
    def act(state):
        return []

class WildCantor(Card):
    color = 'redgreen'
    def act(state):
        out = []
        if state.redgreen > 0:
            out += [dataclasses.replace(state, redgreen=state.redgreen-1, black=state.black+1)]
        return out
        
class LotusPetal(Card):
    def act(state):
        return [dataclasses.replace(state, black=state.black+1), 
                dataclasses.replace(state, redgreen=state.redgreen+1), 
                dataclasses.replace(state, bargain=state.bargain+1)]

class SummonersPact(Card):
    color = 'redgreen'
    def act(state):
        out = [dataclasses.replace(state, redgreen=state.redgreen+1)]

        if state.redgreen > 0:
            out += [dataclasses.replace(state, redgreen=state.redgreen-1, black=state.black+1)]

        return out

class ChromeMox(Card):
    def act(state):
        out = []
        for i in range(len(state.cards)):
            if state.cards[i].color is not None:
                new_cards = [state.cards[j] for j in range(len(state.cards)) if j != i]
                if state.cards[i].color == 'black':
                    out.append(dataclasses.replace(state, black=state.black+1, cards=new_cards))
                if state.cards[i].color == 'redgreen':
                    out.append(dataclasses.replace(state, redgreen=state.redgreen+1, cards=new_cards))
                if state.cards[i].color == 'blue':
                    out.append(dataclasses.replace(state, blue=state.blue+1, cards=new_cards))

        out.append(dataclasses.replace(state, landdrops=state.bargain+1))

        return out
    
class Necro(Card):
    color = 'black'
    def act(state):
        out = []
        if state.black > 2:
            out += [dataclasses.replace(state, black=state.black-3, win=True)]
        return out

class Beseech(Card):
    color = 'black'
    def act(state):
        out = []        

        # With bargain lines
        if state.bargain > 0 and state.black > 3:
            out += [dataclasses.replace(state, black=state.black-4, bargain=state.bargain-1, win=True)]
        if state.bargain > 0 and state.black > 2 and state.redgreen > 0:
            out += [dataclasses.replace(state, black=state.black-3, redgreen=state.redgreen-1, bargain=state.bargain-1, win=True)]
        if state.bargain > 0 and state.black > 2 and state.blue > 0:
            out += [dataclasses.replace(state, black=state.black-3, redgreen=state.blue-1, bargain=state.bargain-1, win=True)]

        # Without bargain lines
        if state.black > 6:
            out += [dataclasses.replace(state, black=state.black-7, win=True)]
        if state.black > 5 and state.redgreen > 0:
            out += [dataclasses.replace(state, black=state.black-6, redgreen=state.redgreen-1, win=True)]
        if state.black > 5 and state.blue > 0:
            out += [dataclasses.replace(state, black=state.black-6, blue=state.blue-1, win=True)]

        return out

class Manamorphose(Card):
    color = 'redgreen'
    def act(state):
        out = []
        if state.redgreen > 0 and state.black > 0:
            out += [dataclasses.replace(state, redgreen=state.redgreen-1, black=state.black+1)]
        if state.redgreen > 1 and state.blue > 0:
            out += [dataclasses.replace(state, redgreen=state.redgreen-1, blue=state.blue-1, black=state.black+2)]
        if state.redgreen > 1:
            out += [dataclasses.replace(state, redgreen=state.redgreen-2, black=state.black+2)]

        return out

class Borne(Card):
    color = 'blue'
    def act(state):
        return []

class PactofNegation(Card):
    color = 'blue'
    def act(state):
        return []

class Tendrils(Card):
    def act(state):
        return []

deck = [
    DarkRitual,
    DarkRitual,
    DarkRitual,
    DarkRitual,
    CabalRitual,
    CabalRitual,
    CabalRitual,
    CabalRitual,
    SpiritGuide,
    SpiritGuide,
    SpiritGuide,
    SpiritGuide,
    SpiritGuide,
    SpiritGuide,
    SpiritGuide,
    SpiritGuide,
    VaultOfWhispers,
    VaultOfWhispers,
    VaultOfWhispers,
    VaultOfWhispers,
    GemstoneMine,
    GemstoneMine,
    GemstoneMine,
    GemstoneMine,
    Valakut,
    Valakut,
    WildCantor,
    LotusPetal,
    LotusPetal,
    LotusPetal,
    LotusPetal,
    ChromeMox,
    ChromeMox,
    ChromeMox,
    ChromeMox,
    Necro,
    Necro,
    Necro,
    Necro,
    Beseech,
    Beseech,
    Beseech,
    Beseech,
    Manamorphose,
    Manamorphose,
    Manamorphose,
    Manamorphose,
    Borne,
    Borne,
    Borne,
    Borne,
    SummonersPact,
    SummonersPact,
    SummonersPact,
    SummonersPact,
    Tendrils,
    PactofNegation,
    PactofNegation,
    PactofNegation,
    PactofNegation,
]

random.shuffle(deck)

state = BoardState(cards=deck[:7])

def recurse(state, winning_states):
    if state.win:
        winning_states.append(state)
        return
    
    for i in range(len(state.cards)):
        new_cards = [state.cards[j] for j in range(len(state.cards)) if j != i]
        new_state = dataclasses.replace(state, cards=new_cards)
        new_states = state.cards[i].act(new_state)

        for new_state in new_states:
            recurse(new_state, winning_states)

n = 100000
win_rates = [0,0,0,0,0,0,0]

for _ in range(n):
    random.shuffle(deck)
    state = BoardState(cards=deck[:7])
    winning_states = []
    recurse(state, winning_states)

    if len(winning_states) == 0:
        continue

    cards_left = 0
    for winning_state in winning_states:
        cards_left = max(cards_left, len(winning_state.cards))

    # if len(winning_states) == 0:
    #     print("Does not win")
    # else:
    #     print("Wins with", cards_left, "cards left")
        
    
    cards_used = 6 - cards_left
    for i in range(cards_used, 7):
        win_rates[i] += 1


p_win = [win_rate/n for win_rate in win_rates]

p_win_total = \
    p_win[6] + \
    (1-p_win[6])*p_win[5] + \
    (1-p_win[6])*(1-p_win[5])*p_win[4] + \
    (1-p_win[6])*(1-p_win[5])*(1-p_win[4])*p_win[3] + \
    (1-p_win[6])*(1-p_win[5])*(1-p_win[4])*(1-p_win[3])*p_win[2] + \
    (1-p_win[6])*(1-p_win[5])*(1-p_win[4])*(1-p_win[3])*(1-p_win[2])*p_win[1] + \
    (1-p_win[6])*(1-p_win[5])*(1-p_win[4])*(1-p_win[3])*(1-p_win[2])*(1-p_win[1])*p_win[0]

print("Win rates:")
print("1 card:", p_win[0])
print("2 cards:", p_win[1])
print("3 cards:", p_win[2])
print("4 cards:", p_win[3])
print("5 cards:", p_win[4])
print("6 cards:", p_win[5])
print("7 cards:", p_win[6])
print("Total win rate:")
print(p_win_total)