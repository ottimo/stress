module Exceptions
	class MissingParameters < StandardError; end
	class InvalidLibraryId < StandardError; end
	class LibraryAlreadyPresent < StandardError; end
	class LibraryNotEmpty < StandardError; end
	class NotOwner < StandardError; end
	class NotSharer < StandardError; end
	class FileNotFound < StandardError; end
	class FileAlreadyShared < StandardError; end
	class InvalidPermission < StandardError; end
	class FileFound < StandardError; end
	class NotZipFile < StandardError; end
	class MalformedPlugin < StandardError; end
	class FileExists < StandardError; end
	class InvalidURIError < StandardError; end	
	class InvalidIPProvided < StandardError; end	
	class InvalidPortProvided < StandardError; end
end
