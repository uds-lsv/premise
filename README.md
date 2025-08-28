# Label-Descriptive Patterns and Their Application to Characterizing Classification Errors

This repository provides the Additional Material for the ICML'22 publication of [Label-Descriptive Patterns and Their Application to Characterizing Classification Errors](https://arxiv.org/abs/2110.09599). 

**If you want to use the Premise algorithm to automatically identify patterns or explanations of where your machine learning classifier performs well and where it fails, you can also try our easy-to-use implementation [PyPremise](https://github.com/uds-lsv/PyPremise) which can be directly included in your Python code and supports different data formats including tokenized text.**

## Abstract

State-of-the-art deep learning methods achieve human-like performance on many tasks, but make errors nevertheless. Characterizing these errors in easily interpretable terms gives insight into whether a classifier is prone to making systematic errors, but also gives a way to act and improve the classifier. We propose to discover those feature-value combinations (ie. patterns) that strongly correlate with correct resp. erroneous predictions to obtain a global and interpretable description for arbitrary classifiers. We show this is an instance of the more general label description problem, which we formulate in terms of the Minimum Description Length principle. To discover a good pattern set, we develop the efficient Premise algorithm. Through an extensive set of experiments we show it performs very well in practice on both synthetic and real-world data. Unlike existing solutions, it ably recovers ground truth patterns, even on highly imbalanced data over many features. Through two case studies on Visual Question Answering and Named Entity Recognition, we confirm that Premise gives clear and actionable insight into the systematic errors made by modern NLP classifiers. 

## Code

- **For Linux (tested on Ubuntu 18.04, requires at least C++14):**  
  - Compile Premise by running `make` in the `code` directory.  
  - You may also be able to use the pre-compiled versions in the `build` directory.  

- **For macOS (Apple M1 chips):**  
  - Necessary adaptations for compilation can be found in these [notes](https://github.com/uds-lsv/premise/blob/master/builds/compile_notes_m1.txt).  

- **For Windows:**  
  - Please see the [Windows Compilation Guide](Compiling_premise_in_Windows.pdf). 
Once you have built Premise, you can then run the method via the command

```
./Premise path_to_features.dat path_to_labels.dat path_to_output.results path_to_embeddings.dat embedding_size num_max_neighbors
```

For the data formats, see the next section. If you do not have embeddings, set num_max_neighbors to 0. It will then not use the embedding neighborhoods and you can then set the path_to_embeddings.dat to an empty file and embedding_size to 0. This implementation can then not generate "or"-clauses but the "and"-patterns will work just fine.

This uses the default parameters as in the paper. You can also change these via

```
./Premise path_to_features.dat path_to_labels.dat path_to_output.results path_to_embeddings.dat embedding_size num_max_neighbors fisher_p_value clause_max_overlap min_overlap
```

where fisher_p_value is the acceptance threshold for the filtering (default 0.01), clause_max_overlap is the maximum overlap allowed between clauses (default 0.05) and min_overlap is the required minimum overlap for items to be considered for the merge (default 0.3).

## Data Format

As input data, our method expects parallel files for features and labels. Each line represents one instance. We use a sparse representation, i.e. only the features/items that occur in an instance are mentioned. For the labels, 0 means misclassification and 1 means correct classification. For a dataset with two instances that have the features [1, 3, 5] and [0, 2, 3] where the first one is classified correclty and the second is misclassified, the files would look like:

features.dat
```
1 3 5
0 2 3
```

labels.dat
```
1
0
```

You can take a look at the data directory for more examples.

For the embeddings, it expects one embedding per line (line 0 = embedding for item/feature 0) where the entries of the embedding vector are space separated in the line, e.g. for two items and example embeddings of dimensionality 4:

embeddings.dat
```
1.5 2.3 0.4 1.3
0.2 -1.3 0.2 0.5
```

In our experiments, we used English FastText embeddings ([cc.en.300.bin](https://fasttext.cc/docs/en/crawl-vectors.html)) with embedding_size 300.

The output file will contain one pattern per line along with additional information about each pattern (separated by ;). The additional information is: counts, direction of the bias (misclassification=0, correct classification=1), fisher value, mdl gain and at which part of the algorithm this pattern was created (e.g. as singleton or as pair).

## Datasets

In the data/misclass_data directory, we provide the feature and label files for the VQA and NER experiments as well as the mapping from item indices to tokens.

In data/found_patterns, you can find the full list of patterns found by all baselines and our method for the VQA and NER experiments.

## Citation

If you use Premise in your work, we would be happy if you tell us about it!

Also, please cite our work as

```
@inproceedings{premise,
  author    = {Michael A. Hedderich and
               Jonas Fischer and
               Dietrich Klakow and
               Jilles Vreeken},
  title     = {Label-Descriptive Patterns and Their Application to Characterizing
               Classification Errors},
  booktitle = {International Conference on Machine Learning, {ICML}},
  series    = {Proceedings of Machine Learning Research},
  year      = {2022},
  url       = {https://proceedings.mlr.press/v162/hedderich22a.html}
}
```

The code of Premise is based on the implementation of [Grab](http://eda.mmci.uni-saarland.de/prj/grab/).

## Contact and Help
If you run into any issues, feel free to contact us (email in the paper) or create an issue on GitHub. We are happy to help you out!

