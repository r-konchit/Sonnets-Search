#!/usr/bin/env python3

class Customer:
    def __init__(self, email, name, shoesize, food):
        self.email = email
        self.name = name
        self.shoesize = shoesize
        self.food = food
    def __str__(self):
        return ("email: {}\nname: {}\nshoesize: {}\nfood: {}\n".format(
            self.email, self.name, self.shoesize, self.food))

def main():
    customers = {}

    with open("customers.tsv") as infile:
        # how to do this in C? fgets
        for line in infile:
            # remove the newline in C?
            line = line.strip()
            # how to find tab characters in C?
            # maybe strchr?
            fields = line.split('\t')
            email = fields[0]
            name = fields[1]
            shoesize = int(fields[2])
            food = fields[3]
            customer = Customer(email, name, shoesize, food)

            customers[email] = customer

    while True:
        command = input("command: ")

        if command == "quit":
            break
        elif command == "save":
            # how to do this in C?
            # open it with fopen in C (for writing here)
            with open("customers.tsv", "w") as outfile:
                for email,customer in customers.items():
                    line = "{}\t{}\t{}\t{}".format(customer.email,
                                                   customer.name,
                                                   customer.shoesize,
                                                   customer.food)
                    print(line, file=outfile)
            # remember to fclose in C
        elif command == "add":
            # Note that in your program, you're going to have to consider the
            # possibility that the user might type a tab character here; you
            # should complain if they do, or potentially just replace the tab
            # character with something else. This Python code doesn't handle
            # that on its own.
            email = input("email address? ")
            name = input("name? ")
            shoesize = int(input("shoe size? "))
            food = input("favorite food? ")
            customer = Customer(email, name, shoesize, food)
            customers[email] = customer
        elif command == "lookup":
            email = input("email address? ")
            if email in customers:
                print(customers[email])
            else:
                print("user not found!")
        elif command == "delete":
            email = input("email address? ")
            if email in customers:
                del customers[email]
            else:
                print("user not found!")
        elif command == "list":
            for email,customer in customers.items():
                print(customer)
        else:
            print("unknown command")

if __name__ == "__main__": main()
