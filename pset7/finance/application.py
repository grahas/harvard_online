from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    if request.method == "GET":
        shares = db.execute("SELECT Symbol, SUM(shares), SUM(price) FROM History WHERE UserId=:ID AND Symbol not like 'cash' GROUP BY Symbol", ID=session["user_id"])
        user_cash_balance = db.execute("SELECT cash FROM users WHERE id=:ID", ID=session["user_id"])
        total_stock_value = db.execute("SELECT sum(Price) FROM History WHERE UserId=:ID GROUP BY Symbol", ID=session["user_id"])
        grand_total = user_cash_balance[0]['cash']
        current_prices = []
        for stock in shares:
            grand_total += stock['SUM(price)'] * stock['SUM(shares)']
            stock['price'] = lookup(stock['Symbol'])
        return render_template("index.html", share=shares, total=grand_total, cash=user_cash_balance)
    return apology("TODO")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "GET":
        return render_template("buy.html")
    elif request.method == "POST":
        if not request.form.get("get_stock"):
            return apology("Invalid Stock Symbol")
        elif not request.form.get("get_shares"):
            return apology("Invalid Stock Symbol")
        else:
            stock = lookup(request.form.get("get_stock"))
            shares = request.form.get("get_shares")
        #can user afford it
        ID=session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=ID)
        if cash[0]['cash'] > int(shares) * int(stock['price']):
            #update portfolio
            db.execute("UPDATE users SET cash = cash - :stock_price * :shares WHERE id = :id", id=session["user_id"], stock_price=stock['price'], shares=int(shares))
            #update cash
            db.execute("INSERT INTO History (UserId, Symbol, Price, shares) VALUES (:user, :Symbol, :Price, :shares)", user=ID, Symbol=stock['symbol'], Price=stock['price'], shares=int(shares))
            #result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form.get("username"),hash=hash_word)
            return redirect(url_for("index"))
        else:
            return apology("Not enough money")
    else:
        return apology("TODO")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    if request.method == "GET":
        rowz = db.execute("SELECT * FROM History WHERE UserId=:id", id=session['user_id'])

        return render_template("history.html", rows=rowz)
    else:
        return apology("BLEH")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted 
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))
    
    
@app.route("/quote_info", methods=["GET", "POST"])
@login_required
def quote_info(stock_dict):
    return render_template("quote_info.html", quote_price=stock_dict['price'], quote_name=stock_dict['name'], quote_symbol=stock_dict['symbol'] )
    
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    elif request.method == "POST":
        if request.form.get("stock_symbol"):
            stock_dict = lookup(request.form.get("stock_symbol"))
            return quote_info(stock_dict)
        else:
            return apology("Info not valid")
    return apology("Info not valid")
    
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    #via post    
    if request.method == "POST":
        #borrow from login.html
        if not request.form.get("username"):
            return apology("must provide username")
    #validate password
        elif not request.form.get("password"):
            return apology("must provide password")
        elif not request.form.get("password") == request.form.get("password2"):
            return apology("passwords do not match") 
    #add user to database
        hash_word = pwd_context.encrypt(request.form.get("password"))
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form.get("username"),hash=hash_word)
        if not result:
            return apology("User alread exist")
    #display form
    else:
        return render_template("register.html")
    return redirect(url_for("index"))

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "GET":
        return render_template("sell.html")
    elif request.method == "POST":
        if not request.form.get("get_stock"):
            return apology("Invalid Stock Symbol")
        elif not request.form.get("get_shares"):
            return apology("Invalid shares")
        else:
            stock = lookup(request.form.get("get_stock"))
            limit = db.execute("SELECT Symbol, SUM(shares) FROM History WHERE UserId=:ID GROUP BY Symbol", ID=session["user_id"])
            shares = request.form.get("get_shares")
            for item in limit:
                if item['Symbol'] is request.form.get("get_stock"):
                    if int(shares) > limit['SUM(shares)']:
                        return apology("You don't have that many shares")
        
        #can user has thoes 
        ID=session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=ID)
        #update portfolio
        db.execute("UPDATE users SET cash = cash + :stock_price * :shares WHERE id = :id", id=session["user_id"], stock_price=stock['price'], shares=int(shares))
        #update cash
        db.execute("INSERT INTO History (UserId, Symbol, Price, shares) VALUES (:user, :Symbol, :Price, :shares)", user=ID, Symbol=stock['symbol'], Price=stock['price'], shares=-1*int(shares))
        #result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form.get("username"),hash=hash_word)
        return render_template("index.html")
    else:
        return apology("TODO")
        
@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    if request.method == "GET":
        return render_template("add_cash.html")
    if request.method == "POST":
        request.form.get("username")
        request.form.get("cardnumber")
        request.form.get("expiry_month")
        request.form.get("password_confirm")
        amount = request.form.get("Amount")
        if not amount.isalpha():
            db.execute("INSERT INTO History (UserId, Symbol, Price, shares) VALUES (:user, :Symbol, :Price, :shares)", user=session["user_id"], Symbol="cash", Price=amount, shares=1)
        else:
            return apology("Invalid input")
        return redirect(url_for("index"))