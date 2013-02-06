# Be sure to restart your server when you modify this file.

# Your secret key for verifying cookie session data integrity.
# If you change this key, all old sessions will become invalid!
# Make sure the secret is at least 30 characters and all random, 
# no regular words or you'll be exposed to dictionary attacks.
ActionController::Base.session = {
  :key         => '_abnf_validator_session',
  :secret      => 'c69bfe50d73771e0a0de2e5b6e176ddc0eb991e16491d57a2f1bc8f23313d61e8669b0375c1672a934a445af3ff2fa3a7a82c0cc92dd9e88e30e3103119c572b'
}

# Use the database for sessions instead of the cookie-based default,
# which shouldn't be used to store highly confidential information
# (create the session table with "rake db:sessions:create")
# ActionController::Base.session_store = :active_record_store
