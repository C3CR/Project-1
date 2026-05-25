
PROJECT 1 
Go Fish Card Game -- C++ STL Edition
==
Course:    CSC17C - Data Structures C++
Professor: Lehr
Student:   Cesar
Date:      Spring 2026
==



------------------------------------------------------------
SECTION 1: INTRODUCTION -- What Game Did I Choose and Why?
------------------------------------------------------------

I chose to implement Go Fish as my project game. Go Fish is a classic card
game where players try to collect complete sets of four cards (called "books")
by asking other players for cards. The player with the most books at the end wins.

Why Go Fish? Because almost every part of the game maps naturally to an STL
container. When I was planning the project, I went through the required
containers one by one and asked "where does this show up in Go Fish?" The answer
was clear every single time:

  - The DECK is a queue (you draw from the front, like a FIFO line)
  - The player's HAND is a list (you add and remove cards all the time)
  - Tracking HOW MANY of each rank you have is a map (rank -> count)
  - Your COMPLETED BOOKS are a set (unique ranks, no duplicates)
  - The DISCARD PILE (for completed books) is a stack (last in, first out)

That's 5 containers, all fitting naturally. No forcing required.

I've never coded a full card game before in C++, so this was a challenge.
I learned a lot about how iterators work differently on different containers,
which actually caused a compile error (random_shuffle doesn't work on list
because list only has bidirectional iterators, not random-access). I had to
use a plain array for the shuffle instead. That bug taught me more about
iterator categories than any lecture could!


------------------------------------------------------------
SECTION 2: GAME RULES
------------------------------------------------------------

Go Fish is played with a standard 52-card deck.

SETUP:
  - Each player is dealt 7 cards (2 players) or 5 cards (3-4 players)
  - Remaining cards form the draw pile (deck)

ON YOUR TURN:
  1. Choose any opponent to ask
  2. Ask them for a rank you already have in your hand
     (You MUST have at least one card of that rank to ask)
  3. If they HAVE cards of that rank: they give you ALL of them
  4. If they DON'T: they say "Go Fish!" and you draw from the deck
     - If you draw the rank you asked for, you take another turn!
     - Otherwise your turn ends

BOOKS:
  - When you collect all 4 cards of a rank (e.g., all four 7s),
    that's a "book"
  - Books score 1 point each and are removed from your hand

GAME END:
  - Game ends when the deck is empty AND at least one player has no cards
  - Player with the most books wins

IN MY IMPLEMENTATION:
  - Player 1 (you) is a human player
  - All other players are computer-controlled
  - Computer uses a greedy strategy: ask for the rank you have the most of


------------------------------------------------------------
SECTION 3: PROJECT STRUCTURE
------------------------------------------------------------

The project is split across 5 files:

  Card.h    -- The Card struct (rank + suit, display methods)
  Deck.h    -- The Deck class (queue<Card> draw pile, shuffle logic)
  Player.h  -- The Player class (list, map, set, stack all used here)
  Game.h    -- The Game class (game loop, turn management, scoring)
  main.cpp  -- Entry point (just creates Game and calls playGame())

This structure mirrors the example from the reference gist (Card.h, Player.h,
main.cpp) but expanded significantly for the STL requirements.

Total lines: 947


------------------------------------------------------------
SECTION 4: STL CONTAINERS USED AND WHY
------------------------------------------------------------

1. LIST<Card> -- Player's Hand (Player.h)
   ----------------------------------------
   The player's hand is stored as a list<Card>.
   
   WHY LIST and not something else?
   - We frequently INSERT cards (drawing, receiving from opponent)
   - We frequently REMOVE cards (giving away, completing books)
   - Removal from the middle of a list is O(1) once you have an iterator
   - No random access needed (we don't say "give me the 3rd card",
     we say "give me all cards of rank 7")
   
   ANALOGY: Think of your hand of cards like a chain of paper clips.
   You can easily add or remove a clip from anywhere in the chain.
   That's a linked list!
   
   Where in code: Player.h, private member "hand"
   Key operations: push_back (draw), erase (give away), iteration

2. MAP<int, int> -- Rank Count Tracker (Player.h)
   ------------------------------------------------
   Each player has a map where the KEY is a card rank (1-13) and
   the VALUE is how many cards of that rank they currently hold.
   
   Example: map[7] = 3 means "I have three 7s"
   
   WHY MAP?
   - Looking up "do I have any 7s?" is O(log n) with map
   - Easy to update: just increment or decrement map[rank]
   - When we check for books, we just scan the map for values >= 4
   
   ANALOGY: Like a pharmacy inventory system -- medication name
   (rank) maps to quantity in stock (count). When stock hits zero,
   it's gone from your hand. When it hits 4, you've got a "book."
   
   Where in code: Player.h, private member "rankCount"
   Key operations: operator[] (update), find() (check if rank exists),
                   iterator traversal (check for books)

3. SET<int> -- Completed Books (Player.h)
   ----------------------------------------
   When a player completes a book (all 4 of a rank), that rank is
   added to a set<int>.
   
   WHY SET?
   - A set only stores unique values -- you can't accidentally
     record the same book twice
   - Checking "did I already book this rank?" is O(log n) with count()
   - The set stays sorted automatically (ranks 1-13 in order)
   
   ANALOGY: Like a checklist where each item can only be checked off
   once. Once "7s" is checked, checking it again does nothing.
   
   Where in code: Player.h, private member "completedBooks"
   Key operations: insert() (mark book complete), count() (check if booked)

4. STACK<Card> -- Discard Pile (Player.h)
   -----------------------------------------
   When a book is completed, the 4 cards go to a discard stack.
   
   WHY STACK?
   - A physical discard pile is LIFO -- most recently discarded cards
     are on top. Stack models this perfectly.
   - We push all 4 book cards onto the stack
   - When we print the discard pile, most recent book is shown first
   
   ANALOGY: Like a pile of graded homework on a teacher's desk. The
   most recently turned-in paper is always on top. You can't see the
   bottom without removing everything above it.
   
   Where in code: Player.h, private member "discardPile"
   Key operations: push(), top(), pop(), empty()

5. QUEUE<Card> -- Draw Pile / Deck (Deck.h)
   -------------------------------------------
   The main deck of 52 cards is stored as a queue.
   
   WHY QUEUE?
   - Drawing from the top of the deck = removing from the FRONT = dequeue
   - In a real game, drawn cards come from the top (front) and the deck
     doesn't get reshuffled in Go Fish
   - FIFO perfectly models the draw pile
   
   ANALOGY: Like a line at a hospital check-in desk. The first patient
   in line is seen first (FIFO). New patients join at the back.
   
   Where in code: Deck.h, private member "drawPile"
   Key operations: push (load), front() (peek), pop() (draw), empty()

6. LIST<Player*> -- Player Turn Order (Game.h)
   -----------------------------------------------
   The Game class stores all players in a list<Player*> to manage turns.
   
   WHY LIST?
   - Easy to iterate in order for each round of turns
   - If a player has no cards and can't draw, they could be removed
     from the active list (list removal is O(1) with iterator)
   
   Where in code: Game.h, private member "players"
   Key operations: push_back(), begin()/end() iteration

7. MAP<string, int> -- Final Scoreboard (Game.h)
   -------------------------------------------------
   At the end of the game, a map<string,int> is built for the scoreboard.
   
   WHY MAP?
   - Ties player name (string) to their final book count (int)
   - Sorted automatically by name for clean display
   - Easy to find the max-score player with a manual max search
   
   Where in code: Game.h, displayFinalResults(), member "scoreboard"


------------------------------------------------------------
SECTION 5: ITERATORS USED
------------------------------------------------------------

I used iterators throughout the code to traverse all STL containers.
This is a summary of each one:

1. list<Card>::iterator -- in Player.h, giveCardsOfRank()
   Walks through the hand list to find and erase matching cards.
   This demonstrates bidirectional iterator (can go ++ and --).

2. list<Card>::const_iterator -- in Player.h, printHand()
   Read-only walk through the hand to display each card.

3. map<int,int>::iterator -- in Player.h, checkForBooks()
   Walks through rankCount map to find ranks with count >= 4.
   Map iterator gives us pairs (first = key, second = value).

4. map<int,int>::const_iterator -- in Player.h, hasRank()
   Read-only map lookup using find(), then checking ->second.

5. set<int>::const_iterator -- in Player.h, printBooks()
   Walks through the set of completed book ranks in sorted order.

6. list<Player*>::iterator -- in Game.h, multiple methods
   Walks through the player list to take turns or deal cards.

7. list<Card>::iterator -- in Deck.h, buildAndShuffle()
   Used to copy the temporary list into the shuffle array.

Iterator categories present in this project:
  - Forward iterators: used in for() loops with ++
  - Bidirectional iterators: list iterators (can do -- too)
  - The compile error I hit with random_shuffle on a list taught me
    that random_shuffle requires RANDOM ACCESS iterators, which
    list does NOT have. Only arrays, deque, and vector have those.
    I fixed this by copying to an array first, then shuffling.


------------------------------------------------------------
SECTION 6: ALGORITHMS USED
------------------------------------------------------------

From <algorithm> header:

1. random_shuffle(deckArray, deckArray + 52)
   - Location: Deck.h, buildAndShuffle()
   - Category: Mutating algorithm
   - Purpose: Randomizes the order of the 52-card deck before dealing
   - Why: Without shuffling, the deck would always be in suit/rank order
     and the game would be completely predictable

2. Manual for_each style iteration (all files)
   - Used throughout to process each element in containers
   - Example: in dealCards(), a for() loop over the player list deals
     one card at a time to each player in rotation

3. max-element style search (Game.h, displayFinalResults)
   - Manual implementation of max_element logic on the scoreboard map
   - Finds the player with the highest book count to declare winner

4. find() equivalent (Player.h, hasRank)
   - Uses map::find() to check if a rank exists in the rankCount map
   - This is O(log n), demonstrating why map is better than linear search


------------------------------------------------------------
SECTION 7: HOW TO COMPILE AND RUN
------------------------------------------------------------

Compile:
  g++ -std=c++11 main.cpp -o gofish

Run:
  ./gofish

On Windows with MinGW:
  g++ -std=c++11 main.cpp -o gofish.exe
  gofish.exe

You will be asked:
  1. How many players (2-4)
  2. Your name (you are Player 1, the human)
  3. Computer players are added automatically

On your turn:
  - You will see your hand displayed
  - You will pick an opponent to ask (by number)
  - You will enter the rank number you want to ask for
    (1=Ace, 2-10=face value, 11=Jack, 12=Queen, 13=King)
  - You MUST have at least one card of that rank to ask for it


------------------------------------------------------------
SECTION 8: SAMPLE INPUT/OUTPUT
------------------------------------------------------------

--- Dealing cards ---
  >> Cesar draws: Jack of Clubs
  >> Computer1 draws: Queen of Spades
  >> Cesar draws: 9 of Hearts
  ...

--- Cards dealt! ---

========== Current Scores ==========
  Cesar: 0 book(s)
  Computer1: 0 book(s)
=====================================

============ YOUR TURN (Cesar) ============

--- Cesar's Hand (7 cards) ---
  1. Jack of Clubs
  2. 9 of Hearts
  3. 6 of Clubs
  4. King of Clubs
  5. 10 of Diamonds
  ...
  [Rank counts: 6:1 9:1 10:1 Jack:1 King:1 ...]

Players you can ask:
  1. Computer1 (7 cards)

Pick a player to ask (1-1): 1
Enter the rank number you want to ask for: 9

Cesar asks Computer1: "Do you have any 9s?"
  Computer1 says: "Go Fish!"
  [Go Fish! Drawing from deck...]
  >> Cesar draws: 9 of Spades

*** Cesar completed a BOOK of 9s! ***    (if they collected all 4)


------------------------------------------------------------
SECTION 9: WHAT I LEARNED
------------------------------------------------------------

1. Iterator categories matter a lot!
   I spent 30 minutes debugging a compile error because I tried to use
   random_shuffle on a list. The fix was simple once I understood WHY --
   random_shuffle needs to jump to any position (random access), and list
   can only go forward/backward one step at a time.

2. map[] vs map.find() are different
   Using map[key] for reading will CREATE the key if it doesn't exist,
   setting the value to 0. I had to use map.find() when I only wanted
   to READ without accidentally creating empty entries.

3. queue and stack have no iterators
   You can't loop through a queue or stack with an iterator. To display
   the discard stack, I had to make a copy and pop it element by element.
   This is intentional -- they are designed for restricted access.

4. list::erase() returns an iterator
   When removing elements from a list inside a loop, you must use the
   return value of erase() as your new iterator position, or you get
   an invalid iterator. This is a common beginner mistake.


------------------------------------------------------------
SECTION 10: CHECKOFF / REQUIREMENTS MET
------------------------------------------------------------

[x] Well-known board/card game with defined rules -- GO FISH
[x] C++ STL library used heavily
[x] Sequence Container -- list<Card> (player hand), list<Player*> (turns)
[x] Associative Container (1) -- map<int,int> (rank counts)
[x] Associative Container (2) -- set<int> (completed books)
[x] Associative Container (3) -- map<string,int> (scoreboard)
[x] Container Adaptor (1) -- stack<Card> (discard pile)
[x] Container Adaptor (2) -- queue<Card> (draw pile)
[x] Iterators used -- list, map, set iterators throughout
[x] Algorithms used -- random_shuffle, for_each style, max search
[x] NO vectors used anywhere
[x] 750+ lines of code -- 947 lines
[x] Clear comments throughout the code
[x] Code compiles with g++ -std=c++11

Files in zip:
  Card.h
  Deck.h
  Player.h
  Game.h
  main.cpp
  writeup.txt


------------------------------------------------------------
SECTION 11: APPLICATION TO DATA SCIENCE AND HEALTHCARE
------------------------------------------------------------

These STL concepts aren't just for games -- they directly apply to
real-world fields:

DATA SCIENCE:
  - map is used in word frequency counting (word -> count), which is the
    basis of text analytics and NLP preprocessing
  - set is used to find unique patient IDs, unique diagnoses, or unique
    medication codes in a dataset
  - queue is used in streaming data pipelines (process records FIFO)
  - Algorithms like sort and find are fundamental to any data analysis

HEALTHCARE OPERATIONS:
  - queue models a patient waiting room (FIFO -- first patient in
    line is seen first)
  - map<patientID, record> is how an EHR (Electronic Health Record)
    system would store patient information for fast lookup
  - stack is used in prescription refill history (most recent refill
    is on top -- LIFO)
  - list models a scheduling queue where appointments can be inserted
    or removed from any position (rescheduling, cancellations)

HEALTHCARE ADMINISTRATION:
  - set is used to track which insurance codes have been billed
    (no duplicates allowed for the same visit)
  - map<doctorID, appointmentList> maps doctors to their schedules
  - Algorithms like max_element find the busiest doctor or the
    highest-cost procedure in a billing cycle
  - Sorting algorithms order patient records by date, priority, or
    insurance status for efficient processing

The same data structures that make Go Fish work cleanly also make
medical record lookup fast, patient queues fair, and billing systems
accurate. That's why learning STL deeply in CSC17C is so valuable.

============================================================
END OF WRITEUP
Cesar -- CSC17C -- Project 1: Go Fish
============================================================

