# Data
We provide two types of data in the additional material:
* misclass_data: the misclassification datasets for VQA and NER
* found_patterns: the patterns found by the different methods 

## misclass_data
All misclassification datasets have the same structure:
* \*_features.dat The features/transaction data, one line per transaction, space separated items
* \*_labels.dat The corresponding labels, 0 for misclassification, 1 for correct classification
* \*_vocabulary.json Mapping between indices (used in \*features.dat) and original words/token
