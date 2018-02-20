# User Manual
The **Decisor** software is a very intuitive and simple tool to model decision problems using AHP.
We will be using a dummy model (very simple) (located at the /Examples folder) and also the model decribed by in the [Wikipedia-AHP](https://en.wikipedia.org/wiki/Analytic_hierarchy_process) page, where a decision model to hire someone is under consideration.

## Start screen
![Start Screen](https://github.com/unisc/decisorGUI/blob/master/images/start-screen.png)

At this point it is possible to:
- Insert an **Objective**
- Insert up to two **Criteria**
- Insert up to two **Alternatives**

If needed more criteria or alternatives, the modeller should press the button **ADD** where the Decisor software will add another criterion or alternative to the model.
If some criterion or alternative needs to be excluded, the modeller should choose one (by clicking at its checkbox) and then pressing the **Delete Selected** button.

It is a good practice to always press the **Update** button after a criterion or alternative is edited. The system will internally updates the screens to reflect the changes (the tab bars).

## Creating a simple model
After entering some text into the input fields, and pressing the **Update** button, the system will automatically create three more tabs on top, one for the criterion-criterion pairwise comparisons, one for the comparisons among alternatives for the first criterion and another one for the second criterion, as shown by next figure (we are using a simplistic *dummy model* of two criteria and two alternatives, just as a simple example):
![Adding criteria and alternatives](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model.png)

It is possible to see the tabs that were created on top. If the user selects the first tab (**Criteria**):
![Criteria selected](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-criteria.png)

To conduct pairwise comparisons for the *Effectiveness* criterion:
![Effectiveness selected](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-effectiveness.png)

To conduct pairwise comparisons for the *Punctuality* criterion:
![Punctuality selected](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-punctuality.png)

If one decides to update the model, i.e., by adding more alternatives, one should go back to the **Model** tab, press **ADD** and then insert *Richard* as a new alternave (for instance):
![New alternative Richard](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-richard.png)

After we added the alternative we have pressed the **Update** button as well. See that the tab **Criteria** is unchanged, however, for the criterion (both), it has added a new Alternative (in this case, *Richard*):
![New alternative Richard](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-richard2.png)

## Pairwise comparisons
Now, we proceed to the pairwise comparisons. In the **Criteria** tab, the modeller should perform judgements among criteria, using a scale. For the sake of our example, we will use the standard Fundamental Scale proposed by Saaty, where elements are entered in a 1 to 9 scale, where 1 is indifferent, and 9 is of high importance. Next table explains the scale in detail:

|*Scale*|*Definition*|*Explanation*|
|:---:|:-------|:------------|
|**1**|Same importance, indifference. |Both elements contribute equally towards the objective.|
|**3**|Small importance one over another. |One element is slightly favored with respect to (W.R.T.) the other.|
|**5**|Considerable importance, essential. |One characteristic is highly important WRT the other.|
|**7**|High importance. |One aspect is strongly more important WRT the other in comparison, dominating the other.|
|**9**|Highest importance, absolute. |Highest importance between two elements, with high level of confidence of its relevance.|
|**2,4,6,8**|Intermediate values among the scale. |A good compromise between two characteristics acting as a reasonable balance.|
|**Reciprocals**|If a compared pair at position (i,j) has value over zero, then the pair at position (j,i) should have assigned 1/value, i.e., a reciprocal value. |The reciprocals values cause the model to have certain equilibrium WRT the elements.|

## Entering pairwise comparisons into the dummy model
We have decided to enter importance by assigning different weights among criteria such as efficiency is considerably more important than punctuality:

![Pairwise judgements](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-criteria-pairwise.png)

The modeller has entered with 5 in the first line, where Effectiveness (line parameter) is of high importance w.r.t. Punctuality. The line represents the comparison w.r.t. criteria x criteria or criteria x alternatives, i.e., if Effectiveness is more important than Punctuality, the first line (Effectiveness) should have a weight (in our case, of value equals to 3) wrt to the column (Punctuality). Note that if (i,j)=3, than the software automatically adds (j,i)=0.3333, i.e., the reciprocal value, representing that Punctuality is three times less important than Effectiveness.

Note also that the software has already computed an intermediary weight vector, despite the fact that the pairwise comparisons for the alternatives are not yet entered. This could be **swithed off** if desired by clicking in *Settings, Automatic Calculation*. In the dummy model, for the criteria x criteria comparisons, the weight vector corresponds to (Effectiveness,Punctuality)=(0.833333,0.166667), meaning that, to this model, Effectiveness is deemed far more important than Punctuality.

Comparing only criteria x criteria is not sufficient for AHP, one should take into account the effect of each criterion in the alternatives, performing the same pairwise comparisons (for the same judgement scale). Next, we should conduct the judgements for each criterion in respect to the possible alternatives.

**Effectiveness with respect to all alternatives**
![Pairwise judgements for effectiveness](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-effectiveness-pairwise.png)

**Punctuality with respect to all alternatives**
![Pairwise judgements for punctuality](https://github.com/unisc/decisorGUI/blob/master/images/dummy-model-punctuality-pairwise.png)

Note that the software presents the choice for this problem: management should hire **Richard**, despite the fact that *Nancy* is more punctual than other options. This is due to the fact that the company prefers effectiveness over punctuality, as stated in the criteria tab for the model and explained earlier.

The software also computes and shows the **synthesis/composition** matrix and the **Consistency Rates** for the criteria matrix, and for all other criteria under analysis. The literature recommend that the Consistency should be below 0.10 (10%), otherwise the judgements were inconsistent. For this cases, the best practice is to review the model with the domain experts or stakeholders and discuss the assigned weights, adjusting the model as they see appropriate.

## Working with more sophisticated models
In next figure, we have clicked in *Open*, then selected the *Examples* folder and then we have choosed the **leader.txt** model):
![Leader Model](https://github.com/unisc/decisorGUI/blob/master/images/leader-model.png)

The **Leader Model** have *four* criteria (Experience, Education, Charisma, Age) and *three* alternatives (Tom, Dick, Harry) for a hiring process within a company.

![Leader model criteria](https://github.com/unisc/decisorGUI/blob/master/images/leader-model-criteria.png)

Next, we present the criteria x criteria pairwise comparisons:

![Leader model - pairwise judgements for criteria](https://github.com/unisc/decisorGUI/blob/master/images/leader-model-criteria-pairwise.png)

Note that according to the input weights, **Experience** has a high value (0.539) in contrast with the rest of the criteria. This demonstrates the relative importance of this criterion in relation to the others. It remains the comparisons among each criterion to the alternatives, as follows:

![Leader model - pairwise judgements for experience](https://github.com/unisc/decisorGUI/blob/master/images/leader-model-experience-pairwise.png)

![Leader model - pairwise judgements for education](https://github.com/unisc/decisorGUI/blob/master/images/leader-model-education-pairwise.png)

![Leader model - pairwise judgements for charisma](https://github.com/unisc/decisorGUI/blob/master/images/leader-model-charisma-pairwise.png)

![Leader model - pairwise judgements for age](https://github.com/unisc/decisorGUI/blob/master/images/leader-model-age-pairwise.png)

The software allows criteria and alternatives weights to be edited, and it also allows the definition of new models (i.e. having more criteria or alternatives), where the **Save as** option will create a new model file for analysis.


