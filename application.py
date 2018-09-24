import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():

    """Show portfolio of stocks"""
    # Define table headers, portfolio list to populate.
    headers = ["Name", "Symbol", "Current Price", "Shares Owned", "Value"]
    portfolio = list()
    sess = session["user_id"]

    # Get stocks, calculate values for each stock.
    history = db.execute("SELECT DISTINCT(company) FROM transactions WHERE id=:i", i = sess)

    for transaction in history:
        symb = transaction["company"]
        name = lookup(symb)["name"]
        current_price = lookup(symb)["price"]

        # If buy or sell transasctions empty, bough/sold = 0. Otherwise, sum columns from transactions.
        if not db.execute("SELECT * FROM transactions WHERE id=:i AND transaction_type='buy' AND company=:c", i=session["user_id"], c=symb):
            bought = 0
        else:
            bought = db.execute("SELECT SUM(quantity) FROM transactions WHERE id=:i AND transaction_type='buy' AND company=:c", i=session["user_id"], c=symb)[0]["SUM(quantity)"]

        if not db.execute("SELECT * FROM transactions WHERE id=:i AND transaction_type='sell' AND company=:c", i=session["user_id"], c=symb):
            sold = 0
        else:
            sold = db.execute("SELECT SUM(quantity) FROM transactions WHERE id=:i AND transaction_type='sell' AND company=:c", i=session["user_id"], c=symb)[0]["SUM(quantity)"]

        # Calculate current shares quantity and value.
        owned = bought - sold
        value = owned * current_price

        # Finally, populate the row (list item) with the values (columns).
        row = name, symb, usd(current_price), owned, usd(value)
        portfolio.append(row)

    # Get user's current cash holdings.
    holdings = float(db.execute("SELECT cash FROM users WHERE id=:u", u=sess)[0]["cash"])

    # Calculate total value of owned stocks.
    stock_value = 0
    for i in range(len(portfolio)):
        stock_value = stock_value + portfolio[i][4]

    # Calculate total value of holdings + all stocks owned.
    total_value = holdings + stock_value

    return render_template("index.html", headers=headers, portfolio=portfolio, holdings=usd(holdings), total_value=usd(total_value))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("buy_num") or not request.form.get("symbol"):
            return apology("Invalid ticker symbol or quantity.", 403)

        #Shorthand upcoming calculations by storing values in variables first.
        sess = session["user_id"]
        holdings = float(db.execute("SELECT cash FROM users WHERE id=:u", u=sess)[0]["cash"])
        buy_num = int(request.form.get("buy_num"))
        price = float(lookup(request.form.get("symbol"))["price"])
        symb = lookup(request.form.get("symbol"))["symbol"]

        # Calculat cost of the transaction.
        cost = price * buy_num

        # Verify user has sufficient funds. If not, apologize. Else, update transaction table and user's holdings.
        if holdings < cost:
            return apology("Sorry, you're too poor.", 777)

        else:
            db.execute("INSERT INTO transactions (id, transaction_type, company, quantity, price) VALUES (:i, :t, :c, :q, :p)", i=sess, t="buy", c=symb, q=buy_num, p=cost)
            update_val = holdings - cost
            db.execute("UPDATE users SET cash=:n_cash WHERE id=:i", n_cash=update_val, i=sess)
            return redirect("/")

    # Otherwise method is GET. Render default template.
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_history = db.execute("SELECT * FROM transactions WHERE id=:i", i = session["user_id"])
    price, transaction_type, company, quantity, timestamp = [], [], [], [], []
    # Format cash values with USD function.
    for transaction in user_history:
        price.append(usd(transaction["price"]))
        transaction_type.append(transaction["transaction_type"])
        company.append(transaction["company"])
        quantity.append(transaction["quantity"])
        timestamp.append(transaction["timestamp"])
        headers = ["Stock", "Buy/Sell", "Quantity", "Price per share", "Date"]
        length = len(price)

    return render_template("history.html", headers=headers, pr=price, tr=transaction_type, co=company, qu=quantity, ti=timestamp, length=length)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not lookup(request.form.get("symbol")):
            return apology("Symbol does not exist", 403)
        quote = lookup(request.form.get("symbol"))
        quote["price"] = usd(quote["price"])
        return render_template("quoted.html", chart=quote)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():

    """Register user"""

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 403)

        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Password and confirmation do not match.", 403)

        if not db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username")):
            db.execute("INSERT INTO users (username, hash) VALUES (:u, :h)", u=request.form.get("username"), h=generate_password_hash(request.form.get("password")))
            rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
            session["user_id"] = rows[0]["id"]
            return render_template("/register.html")
        else:
            return apology("Username already exists.", 405)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        if not request.form.get("sell_num") or not request.form.get("symbol"):
            return apology("Invalid ticker symbol or quantity.", 403)

        #Shorthand upcoming calculations by storing values in variables first.
        price = float(lookup(request.form.get("symbol"))["price"])
        symb = lookup(request.form.get("symbol"))["symbol"]
        sess = session["user_id"]
        sell_num = int(request.form.get("sell_num"))


        if not db.execute("SELECT * FROM transactions WHERE id=:i AND transaction_type='buy' AND company=:c", i=session["user_id"], c=symb):
            bought = 0
        else:
            bought = db.execute("SELECT SUM(quantity) FROM transactions WHERE id=:i AND transaction_type='buy'", i=session["user_id"])[0]["SUM(quantity)"]

        if not db.execute("SELECT * FROM transactions WHERE id=:i AND transaction_type='sell' AND company=:c", i=session["user_id"], c=symb):
            sold = 0
        else:
            sold = db.execute("SELECT SUM(quantity) FROM transactions WHERE id=:i AND transaction_type='sell'", i=session["user_id"])[0]["SUM(quantity)"]

        owned = bought - sold
        holdings = float(db.execute("SELECT cash FROM users WHERE id=:u", u=sess)[0]["cash"])


        # Calculat cost of the transaction.
        cost = price * sell_num

        # Verify user has sufficient number of shares. If not, apologize. Else, update transaction table and user's holdings and shares.
        if owned < sell_num:
            return apology("Sorry, you don't have that many shares.", 777)

        else:
            db.execute("INSERT INTO transactions (id, transaction_type, company, quantity, price) VALUES (:i, :t, :c, :q, :p)", i=sess, t="sell", c=symb, q=sell_num, p=cost)
            update_val = holdings + cost
            db.execute("UPDATE users SET cash=:n_cash WHERE id=:i", n_cash=update_val, i=sess)
            return render_template("/index.html")

    # Otherwise method is GET. Render default template.
    else:
        return render_template("sell.html")


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
