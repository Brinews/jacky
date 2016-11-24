'''
The AutoMagical Procedural Python Potionization Program
'''


from mixer import (
    get_ingredients,  # Example usage: ingredients = get_ingredients("Potion Name")
    add,              # Example usage: cauldron = add(cauldron, ingredient[3])
    stir,             # Example usage: cauldron = stir(cauldron, 5)
    say_incantation,  # Example usage: cauldron = say_incantation(cauldron)
    get_cauldron,     # Example usage: cauldron = get_cauldron()
    render_potion     # Example usage: cauldron = render_potion(cauldron)
)

def make_elixir_to_induce_euphoria():
    '''
    The Elixir to Induce Euphoria is a sunshine-yellow potion that induces
    a sense of inexplicable, irrational happiness upon the drinker.
    '''
    cauldron = get_cauldron()
    ingredients = get_ingredients("Elixir To Induce Euphoria")
    # add the first ingredient into the cauldron

    cauldron = add(cauldron, ingredients[0])

    # add the second ingredient into the cauldron
    cauldron = add(cauldron, ingredients[1])

    # stir four times
    cauldron = stir(cauldron, 4)

    # add the third ingredient into the cauldron
    cauldron = add(cauldron, ingredients[2])

    # add the fourth ingredient into the cauldron
    cauldron = add(cauldron, ingredients[3])

    # add the fifth ingredient into the cauldron
    cauldron = add(cauldron, ingredients[4])

    # stir six times
    cauldron = stir(cauldron, 6)

    # render the cauldron into a potion (we did this for you!)
    potion = render_potion(cauldron)
    return potion



def make_felix_felicis():
    '''
    Felix Felicis, also called "Liquid Luck", is a magical potion that makes
    the drinker lucky for a period of time, during which everything they attempt
    will be successful.   
    '''
    cauldron = get_cauldron()
    ingredients = get_ingredients("Felix Felicis")
    # add the first ingredient into the cauldron

    # add the second ingredient into the cauldron

    # add the third ingredient into the cauldron

    # stir 10 times

    # add the fourth ingredient into the cauldron

    # add the fifth ingredient into the cauldron

    # stir once

    # add the sixth ingredient into the cauldron

    # stir once

    # add the seventh ingredient into the cauldron

    # stir 10 times

    # say incantation

    # render the cauldron into a potion (we did this for you!)
    potion = render_potion(cauldron)
    return potion



def main():
    '''
    Put your code for testing your recipes here.
    '''
    print "Concocting an Elixir to Induce Euphoria!"
    print make_elixir_to_induce_euphoria() 









################################################
# Do not change anything below this line. ######
################################################

if __name__ == '__main__':
    main() 
